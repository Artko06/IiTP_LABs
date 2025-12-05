#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <thread>
#include <string>

// Функция для обработки данных (простой пример - инвертирование байтов)
void ProcessData(BYTE* data, DWORD start, DWORD end) {
    for (DWORD i = start; i < end; i++) {
        data[i] = ~data[i]; // Инвертируем каждый байт
    }
}

// Многопоточная обработка для традиционного метода
BOOL ProcessFileTraditionalMultiThreaded(LPCSTR filename, int num_threads) {
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BYTE* buffer = NULL;
    DWORD fileSize = 0;
    DWORD bytesRead = 0;
    DWORD bytesWritten = 0;

    printf("Традиционный метод (%d потоков): %s\n", num_threads, filename);

    // 1. Открываем файл для чтения
    hFile = CreateFileA(
        filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка: не удалось открыть файл для чтения\n");
        return FALSE;
    }

    // 2. Получаем размер файла
    fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("Ошибка: не удалось получить размер файла\n");
        CloseHandle(hFile);
        return FALSE;
    }

    printf("Размер файла: %lu байт\n", fileSize);

    if (fileSize == 0) {
        printf("Файл пуст\n");
        CloseHandle(hFile);
        return TRUE;
    }

    // 3. Выделяем память под данные
    buffer = (BYTE*)malloc(fileSize);
    if (buffer == NULL) {
        printf("Ошибка: не удалось выделить память\n");
        CloseHandle(hFile);
        return FALSE;
    }

    // 4. Читаем файл
    if (!ReadFile(hFile, buffer, fileSize, &bytesRead, NULL) || bytesRead != fileSize) {
        printf("Ошибка: не удалось прочитать файл\n");
        free(buffer);
        CloseHandle(hFile);
        return FALSE;
    }

    CloseHandle(hFile);

    // 5. Многопоточная обработка данных
    printf("Начало многопоточной обработки данных (%d потоков)...\n", num_threads);

    std::vector<std::thread> threads;
    DWORD chunk_size = fileSize / num_threads;

    for (int i = 0; i < num_threads; i++) {
        DWORD start = i * chunk_size;
        DWORD end = (i == num_threads - 1) ? fileSize : (i + 1) * chunk_size;

        threads.emplace_back([buffer, start, end, i]() {
            printf("Поток %d обрабатывает байты [%lu - %lu]\n", i, start, end - 1);
            ProcessData(buffer, start, end);
            });
    }

    // Ждем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    printf("Многопоточная обработка данных завершена\n");

    // 6. Открываем файл для записи
    hFile = CreateFileA(
        filename,
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка: не удалось открыть файл для записи\n");
        free(buffer);
        return FALSE;
    }

    // 7. Записываем обработанные данные
    if (!WriteFile(hFile, buffer, fileSize, &bytesWritten, NULL) || bytesWritten != fileSize) {
        printf("Ошибка: не удалось записать файл\n");
    }

    // 8. Освобождаем ресурсы
    free(buffer);
    CloseHandle(hFile);

    return TRUE;
}

// Многопоточная обработка для метода отображения в память
BOOL ProcessFileWithMappingMultiThreaded(LPCSTR filename, int num_threads) {
    HANDLE hFile = INVALID_HANDLE_VALUE;
    HANDLE hMapping = NULL;
    BYTE* pData = NULL;
    DWORD fileSize = 0;

    printf("Отображение в память (%d потоков): %s\n", num_threads, filename);

    // 1. Открываем файл
    hFile = CreateFileA(
        filename,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка: не удалось открыть файл. Код ошибки: %lu\n", GetLastError());
        return FALSE;
    }

    // 2. Получаем размер файла
    fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("Ошибка: не удалось получить размер файла\n");
        CloseHandle(hFile);
        return FALSE;
    }

    printf("Размер файла: %lu байт\n", fileSize);

    if (fileSize == 0) {
        printf("Файл пуст\n");
        CloseHandle(hFile);
        return TRUE;
    }

    // 3. Создаем файловое отображение
    hMapping = CreateFileMapping(
        hFile,
        NULL,
        PAGE_READWRITE,
        0,
        0,
        NULL
    );

    if (hMapping == NULL) {
        printf("Ошибка: не удалось создать файловое отображение. Код ошибки: %lu\n", GetLastError());
        CloseHandle(hFile);
        return FALSE;
    }

    // 4. Отображаем файл в память
    pData = (BYTE*)MapViewOfFile(
        hMapping,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        0
    );

    if (pData == NULL) {
        printf("Ошибка: не удалось отобразить файл в память. Код ошибки: %lu\n", GetLastError());
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return FALSE;
    }

    printf("Файл успешно отображен в память по адресу: %p\n", pData);

    // 5. Многопоточная обработка данных
    printf("Начало многопоточной обработки данных (%d потоков)...\n", num_threads);

    std::vector<std::thread> threads;
    DWORD chunk_size = fileSize / num_threads;

    for (int i = 0; i < num_threads; i++) {
        DWORD start = i * chunk_size;
        DWORD end = (i == num_threads - 1) ? fileSize : (i + 1) * chunk_size;

        threads.emplace_back([pData, start, end, i]() {
            printf("Поток %d обрабатывает байты [%lu - %lu]\n", i, start, end - 1);
            ProcessData(pData, start, end);
            });
    }

    // Ждем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    printf("Многопоточная обработка данных завершена\n");

    // 6. Освобождаем ресурсы
    if (!UnmapViewOfFile(pData)) {
        printf("Предупреждение: ошибка при освобождении отображения\n");
    }

    CloseHandle(hMapping);
    CloseHandle(hFile);

    return TRUE;
}

// Функция для создания тестового файла
BOOL CreateTestFile(LPCSTR filename, DWORD size) {
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка создания тестового файла\n");
        return FALSE;
    }

    // Заполняем файл тестовыми данными
    BYTE* testData = (BYTE*)malloc(size);
    if (testData) {
        for (DWORD i = 0; i < size; i++) {
            testData[i] = (BYTE)(i % 256);
        }

        DWORD bytesWritten;
        WriteFile(hFile, testData, size, &bytesWritten, NULL);
        free(testData);
    }

    CloseHandle(hFile);
    printf("Создан тестовый файл: %s, размер: %lu байт\n", filename, size);
    return TRUE;
}

// Функция для копирования файла (для тестирования на одинаковых данных)
BOOL CopyTestFile(LPCSTR source, LPCSTR dest) {
    return CopyFileA(source, dest, FALSE);
}

// Тестирование методов с разным количеством потоков
void TestWithDifferentThreadCounts(const char* base_filename, DWORD fileSize) {
    std::vector<int> thread_counts = { 1, 2, 4, 8 };

    printf("\n=== СРАВНЕНИЕ МЕТОДОВ С РАЗНЫМ КОЛИЧЕСТВОМ ПОТОКОВ ===\n");
    printf("Размер файла: %lu MB\n\n", fileSize / (1024 * 1024));

    // Таблица результатов
    struct Result {
        double traditional_time;
        double mapping_time;
    };

    std::vector<Result> results(thread_counts.size());

    // Тестируем для каждого количества потоков
    for (size_t i = 0; i < thread_counts.size(); i++) {
        int threads = thread_counts[i];
        printf("--- Тестирование с %d потоками ---\n", threads);

        // Создаем копии файлов для тестирования
        char traditional_filename[MAX_PATH];
        char mapping_filename[MAX_PATH];

        sprintf_s(traditional_filename, "traditional_%d.dat", threads);
        sprintf_s(mapping_filename, "mapping_%d.dat", threads);

        CopyTestFile(base_filename, traditional_filename);
        CopyTestFile(base_filename, mapping_filename);

        // Тестируем традиционный метод
        printf("Традиционный метод...\n");
        clock_t start = clock();
        ProcessFileTraditionalMultiThreaded(traditional_filename, threads);
        clock_t end = clock();
        results[i].traditional_time = ((double)(end - start)) / CLOCKS_PER_SEC;

        // Тестируем метод отображения в память
        printf("Отображение в память...\n");
        start = clock();
        ProcessFileWithMappingMultiThreaded(mapping_filename, threads);
        end = clock();
        results[i].mapping_time = ((double)(end - start)) / CLOCKS_PER_SEC;

        // Удаляем временные файлы
        DeleteFileA(traditional_filename);
        DeleteFileA(mapping_filename);

        printf("Завершено\n\n");
    }

    // Выводим сравнительную таблицу (исправленную)
    printf("=== РЕЗУЛЬТАТЫ СРАВНЕНИЯ ===\n");
    printf("===============================================================\n");
    printf("| Количество   | Традиционный    | Отображение     | Выигрыш  |\n");
    printf("| потоков      | метод (сек)     | в память (сек)  |          |\n");
    printf("===============================================================\n");

    for (size_t i = 0; i < thread_counts.size(); i++) {
        double traditional = results[i].traditional_time;
        double mapping = results[i].mapping_time;
        double advantage = traditional / mapping;

        printf("| %12d | %15.3f | %15.3f | %8.2fx |\n",
            thread_counts[i], traditional, mapping, advantage);
    }
    printf("===============================================================\n");

    // Анализ результатов
    printf("\n=== АНАЛИЗ РЕЗУЛЬТАТОВ ===\n");
    double best_traditional = results[0].traditional_time;
    double best_mapping = results[0].mapping_time;
    int best_traditional_threads = 1;
    int best_mapping_threads = 1;

    for (size_t i = 1; i < thread_counts.size(); i++) {
        if (results[i].traditional_time < best_traditional) {
            best_traditional = results[i].traditional_time;
            best_traditional_threads = thread_counts[i];
        }
        if (results[i].mapping_time < best_mapping) {
            best_mapping = results[i].mapping_time;
            best_mapping_threads = thread_counts[i];
        }
    }

    printf("Лучшая производительность традиционного метода: %d потоков (%.3f сек)\n",
        best_traditional_threads, best_traditional);
    printf("Лучшая производительность отображения в память: %d потоков (%.3f сек)\n",
        best_mapping_threads, best_mapping);
    printf("Максимальное ускорение: %.2fx\n", best_traditional / best_mapping);
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const char* base_filename = "base_test_file.bin";
    const DWORD fileSize = 100 * 1024 * 1024;

    printf("=== СРАВНИТЕЛЬНЫЙ АНАЛИЗ МЕТОДОВ ОБРАБОТКИ ФАЙЛОВ ===\n");

    // Создаем базовый тестовый файл
    if (!CreateTestFile(base_filename, fileSize)) {
        printf("Не удалось создать тестовый файл\n");
        return 1;
    }

    // Тестируем оба метода с разным количеством потоков
    TestWithDifferentThreadCounts(base_filename, fileSize);

    DeleteFileA(base_filename);

    printf("\nНажмите Enter для выхода...");
    getchar();

    return 0;
}