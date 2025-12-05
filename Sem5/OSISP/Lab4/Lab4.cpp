#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>
#include <atomic>
#include <condition_variable>
#include <string>
#include <sstream>

// Мьютекс для синхронизации вывода
std::mutex cout_mutex;

void SafePrint(const std::string& message) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << message << std::endl;
}

class ReaderWriter {
private:
    std::mutex mutex_;                  // Основной мьютекс для данных
    std::condition_variable reader_cv_; // Условная переменная для читателей
    std::condition_variable writer_cv_; // Условная переменная для писателей
    std::string shared_data;            // Разделяемые данные

    // Счетчики для управления доступом
    int active_readers;
    int active_writers;
    int waiting_writers;

    // Статистика
    std::atomic<int> successful_reads;
    std::atomic<int> successful_writes;
    std::atomic<int> failed_reads;
    std::atomic<int> failed_writes;
    std::atomic<int> total_reads;
    std::atomic<int> total_writes;

public:
    ReaderWriter() : shared_data("Initial Data"),
        active_readers(0),
        active_writers(0),
        waiting_writers(0),
        successful_reads(0),
        successful_writes(0),
        failed_reads(0),
        failed_writes(0),
        total_reads(0),
        total_writes(0) {
    }

    // Чтение данных (могут работать несколько читателей одновременно)
    void read_data(int reader_id) {
        auto start_time = std::chrono::steady_clock::now();

        {
            std::unique_lock<std::mutex> lock(mutex_);

            // Ждем, пока нет активных писателей
            while (active_writers > 0 || waiting_writers > 0) {
                reader_cv_.wait(lock);
            }

            active_readers++;
        }

        // Критическая секция для чтения
        // Имитация времени чтения
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        // Безопасный вывод
        std::stringstream ss;
        ss << "Reader " << reader_id << " read: '" << shared_data
            << "' | Active readers: " << active_readers
            << " | Wait time: " << duration.count() << "ms";
        SafePrint(ss.str());

        {
            std::unique_lock<std::mutex> lock(mutex_);
            active_readers--;

            // Если это был последний читатель, будим писателей
            if (active_readers == 0) {
                writer_cv_.notify_one();
            }
        }

        successful_reads++;
        total_reads++;
    }

    // Запись данных (только один писатель одновременно)
    void write_data(int writer_id, const std::string& new_data) {
        auto start_time = std::chrono::steady_clock::now();

        {
            std::unique_lock<std::mutex> lock(mutex_);
            waiting_writers++;

            // Ждем, пока нет активных читателей и писателей
            while (active_readers > 0 || active_writers > 0) {
                writer_cv_.wait(lock);
            }

            waiting_writers--;
            active_writers++;
        }

        // Критическая секция для записи
        // Имитация времени записи
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Безопасное преобразование writer_id в строку
        std::stringstream ss_id;
        ss_id << writer_id;
        shared_data = new_data + " (written by " + ss_id.str() + ")";

        auto end_time = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

        // Безопасный вывод
        std::stringstream ss;
        ss << "Writer " << writer_id << " wrote: '" << shared_data
            << "' | Wait time: " << duration.count() << "ms";
        SafePrint(ss.str());

        {
            std::unique_lock<std::mutex> lock(mutex_);
            active_writers--;

            // Будим всех ожидающих
            if (waiting_writers > 0) {
                writer_cv_.notify_one();
            }
            else {
                reader_cv_.notify_all();
            }
        }

        successful_writes++;
        total_writes++;
    }

    // Попытка чтения с таймаутом
    bool try_read_data(int reader_id, int timeout_ms = 100) {
        auto start_time = std::chrono::steady_clock::now();
        bool success = false;

        {
            std::unique_lock<std::mutex> lock(mutex_);

            // Пытаемся получить доступ в течение timeout_ms
            auto timeout_time = std::chrono::steady_clock::now() +
                std::chrono::milliseconds(timeout_ms);

            while ((active_writers > 0 || waiting_writers > 0) &&
                std::chrono::steady_clock::now() < timeout_time) {
                reader_cv_.wait_until(lock, timeout_time);
            }

            if (active_writers == 0 && waiting_writers == 0) {
                active_readers++;
                success = true;
            }
        }

        if (success) {
            // Критическая секция для чтения
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            auto end_time = std::chrono::steady_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

            // Безопасный вывод
            std::stringstream ss;
            ss << "Reader " << reader_id << " read: '" << shared_data
                << "' | Wait time: " << duration.count() << "ms";
            SafePrint(ss.str());

            {
                std::unique_lock<std::mutex> lock(mutex_);
                active_readers--;

                if (active_readers == 0) {
                    writer_cv_.notify_one();
                }
            }

            successful_reads++;
            total_reads++;
        }
        else {
            failed_reads++;
            // Безопасный вывод ошибки
            std::stringstream ss;
            ss << "Reader " << reader_id << " failed to acquire lock (timeout)";
            SafePrint(ss.str());
        }

        return success;
    }

    // Статистика
    void print_statistics() {
        SafePrint("\n=== STATISTICS ===");

        std::stringstream ss1;
        ss1 << "Successful reads: " << successful_reads.load();
        SafePrint(ss1.str());

        std::stringstream ss2;
        ss2 << "Successful writes: " << successful_writes.load();
        SafePrint(ss2.str());

        std::stringstream ss3;
        ss3 << "Failed reads: " << failed_reads.load();
        SafePrint(ss3.str());

        std::stringstream ss4;
        ss4 << "Failed writes: " << failed_writes.load();
        SafePrint(ss4.str());

        std::stringstream ss5;
        ss5 << "Total operations: " << (total_reads.load() + total_writes.load());
        SafePrint(ss5.str());

        int reads = total_reads.load();
        int writes = total_writes.load();
        double ratio = (writes > 0) ? (double)reads / writes : reads;

        std::stringstream ss6;
        ss6 << "Readers/Writers ratio: " << ratio;
        SafePrint(ss6.str());
    }

    // Геттеры для статистики
    int get_successful_reads() const { return successful_reads.load(); }
    int get_successful_writes() const { return successful_writes.load(); }
    int get_failed_reads() const { return failed_reads.load(); }
    int get_failed_writes() const { return failed_writes.load(); }
};

// Вспомогательная функция для преобразования int в string
std::string int_to_string(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

// Функция для читателя
void reader_thread(ReaderWriter& rw, int reader_id, int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(100, 500);

    for (int i = 0; i < iterations; ++i) {
        rw.read_data(reader_id);
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    }
}

// Функция для писателя
void writer_thread(ReaderWriter& rw, int writer_id, int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(200, 800);

    for (int i = 0; i < iterations; ++i) {
        rw.write_data(writer_id, "Update_" + int_to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    }
}

// Функция для читателя с таймаутом
void timed_reader_thread(ReaderWriter& rw, int reader_id, int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(50, 300);

    for (int i = 0; i < iterations; ++i) {
        rw.try_read_data(reader_id, 50); 
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    }
}

int main() {
    ReaderWriter rw;

    SafePrint("=== READER-WRITER MODEL ===");
    SafePrint("Starting simulation with 3 readers and 2 writers...");

    // Параметры симуляции
    const int num_readers = 3;
    const int num_writers = 2;
    const int num_timed_readers = 2;
    const int iterations = 5;

    std::vector<std::thread> threads;

    // Запуск обычных читателей
    for (int i = 0; i < num_readers; ++i) {
        threads.push_back(std::thread(reader_thread, std::ref(rw), i + 1, iterations));
    }

    // Запуск писателей
    for (int i = 0; i < num_writers; ++i) {
        threads.push_back(std::thread(writer_thread, std::ref(rw), i + 1, iterations));
    }

    // Запуск читателей с таймаутом
    for (int i = 0; i < num_timed_readers; ++i) {
        threads.push_back(std::thread(timed_reader_thread, std::ref(rw),
            num_readers + i + 1, iterations));
    }

    // Ожидание завершения всех потоков
    for (size_t i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }

    // Вывод статистики
    rw.print_statistics();

    // Анализ эффективности
    SafePrint("\n=== PERFORMANCE ANALYSIS ===");

    int total_success = rw.get_successful_reads() + rw.get_successful_writes();
    int total_operations = total_success + rw.get_failed_reads() + rw.get_failed_writes();
    double success_rate = (total_operations > 0) ? (double)total_success / total_operations : 0.0;

    std::stringstream ss;
    ss << "Success rate: " << (success_rate * 100) << "%";
    SafePrint(ss.str());

    if (rw.get_failed_reads() > 0) {
        SafePrint("Note: Some reads failed due to timeouts (reader starvation possible)");
    }

    return 0;
}