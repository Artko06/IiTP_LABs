#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <omp.h>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <sched.h>
#include <pthread.h>
#include <hwloc.h>
#include </usr/include/gegl-0.4/opencl/opencl.hpp>
#include <numeric>

#define ITERATIONS 10
#define LINEAR_DATA_SIZE 100000
#define BINARY_DATA_SIZE 10000000

// Глобальная топология hwloc
hwloc_topology_t topology;

// Структура результатов должна быть объявлена ДО классов, которые ее используют
struct SearchResults
{
    long double linear_time;
    long double binary_time;
    size_t linear_found;
    size_t binary_found;
};

// Класс для GPU поиска
class GPUSearcher
{
private:
    cl::Context context;
    cl::CommandQueue queue;
    cl::Program program;
    cl::Kernel linear_search_kernel;
    cl::Kernel binary_search_kernel;
    bool available;

public:
    GPUSearcher() : available(false)
    {
        try
        {
            // Получаем все доступные платформы
            std::vector<cl::Platform> platforms;
            cl::Platform::get(&platforms);

            if (platforms.empty())
            {
                std::cerr << "OpenCL платформы не найдены" << std::endl;
                return;
            }

            // Ищем платформу с iGPU (Intel, AMD, NVIDIA)
            cl::Platform selected_platform;
            cl::Device selected_device;
            bool found_gpu = false;

            for (auto &platform : platforms)
            {
                std::vector<cl::Device> devices;
                platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

                for (auto &device : devices)
                {
                    std::string device_name = device.getInfo<CL_DEVICE_NAME>();
                    std::string platform_name = platform.getInfo<CL_PLATFORM_NAME>();

                    std::cout << "Найдено GPU устройство: " << device_name
                              << " на платформе: " << platform_name << std::endl;

                    // Предпочтение Intel iGPU
                    if (platform_name.find("Intel") != std::string::npos)
                    {
                        selected_platform = platform;
                        selected_device = device;
                        found_gpu = true;
                        std::cout << "Выбрана Intel iGPU" << std::endl;
                        break;
                    }
                    // Или любая другая GPU
                    else if (!found_gpu)
                    {
                        selected_platform = platform;
                        selected_device = device;
                        found_gpu = true;
                        std::cout << "Выбрана GPU: " << device_name << std::endl;
                    }
                }

                if (found_gpu)
                    break;
            }

            if (!found_gpu)
            {
                std::cerr << "GPU устройства не найдены" << std::endl;
                return;
            }

            // Создаем контекст и очередь
            context = cl::Context(selected_device);
            queue = cl::CommandQueue(context, selected_device);

            // Компилируем программу
            std::string kernel_source = R"(
                __kernel void linear_search(__global const int* data, 
                                          __global const int* targets,
                                          __global int* results,
                                          int data_size,
                                          int targets_size) {
                    int tid = get_global_id(0);
                    if (tid >= targets_size) return;
                    
                    int target = targets[tid];
                    int found = 0;
                    
                    for (int i = 0; i < data_size; i++) {
                        if (data[i] == target) {
                            found = 1;
                            break;
                        }
                    }
                    results[tid] = found;
                }
                
                __kernel void binary_search(__global const int* sorted_data,
                                          __global const int* targets,
                                          __global int* results,
                                          int data_size,
                                          int targets_size) {
                    int tid = get_global_id(0);
                    if (tid >= targets_size) return;
                    
                    int target = targets[tid];
                    int left = 0;
                    int right = data_size - 1;
                    int found = 0;
                    
                    while (left <= right) {
                        int mid = left + (right - left) / 2;
                        if (sorted_data[mid] == target) {
                            found = 1;
                            break;
                        } else if (sorted_data[mid] < target) {
                            left = mid + 1;
                        } else {
                            right = mid - 1;
                        }
                    }
                    results[tid] = found;
                }
            )";

            cl::Program::Sources sources;
            sources.push_back({kernel_source.c_str(), kernel_source.length()});

            program = cl::Program(context, sources);
            program.build();

            linear_search_kernel = cl::Kernel(program, "linear_search");
            binary_search_kernel = cl::Kernel(program, "binary_search");
            available = true;

            std::cout << "OpenCL инициализирован успешно" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Ошибка инициализации GPU: " << e.what() << std::endl;
            available = false;
        }
    }

    bool isAvailable() const { return available; }

    SearchResults search_gpu_timed(const std::vector<int> &linear_data,
                                   const std::vector<int> &binary_data,
                                   const std::vector<int> &linear_targets,
                                   const std::vector<int> &binary_targets)
    {
        if (!available)
        {
            std::cerr << "GPU недоступен" << std::endl;
            return {0, 0, 0, 0};
        }

        size_t linear_data_size = linear_data.size();
        size_t binary_data_size = binary_data.size();
        size_t linear_targets_size = linear_targets.size();
        size_t binary_targets_size = binary_targets.size();

        SearchResults results = {0, 0, 0, 0};

        // Линейный поиск на GPU (только для небольших размеров)
        if (linear_targets_size <= 50000)
        {
            cl::Buffer linear_data_buf(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                        linear_data_size * sizeof(int), const_cast<int *>(linear_data.data()));
            cl::Buffer linear_targets_buf(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                            linear_targets_size * sizeof(int), const_cast<int *>(linear_targets.data()));
            cl::Buffer linear_results_buf(context, CL_MEM_WRITE_ONLY,
                                            linear_targets_size * sizeof(int));

            auto linear_start = std::chrono::high_resolution_clock::now();

            linear_search_kernel.setArg(0, linear_data_buf);
            linear_search_kernel.setArg(1, linear_targets_buf);
            linear_search_kernel.setArg(2, linear_results_buf);
            linear_search_kernel.setArg(3, static_cast<int>(linear_data_size));
            linear_search_kernel.setArg(4, static_cast<int>(linear_targets_size));

            queue.enqueueNDRangeKernel(linear_search_kernel, cl::NullRange,
                                        cl::NDRange(linear_targets_size), cl::NullRange);
            queue.finish();

            std::vector<int> linear_results(linear_targets_size);
            queue.enqueueReadBuffer(linear_results_buf, CL_TRUE, 0,
                                    linear_targets_size * sizeof(int), linear_results.data());

            auto linear_end = std::chrono::high_resolution_clock::now();
            results.linear_time = std::chrono::duration_cast<std::chrono::microseconds>(
                                        linear_end - linear_start)
                                        .count();
            results.linear_found = std::accumulate(linear_results.begin(),
                                                    linear_results.end(), 0);
        }

        // Бинарный поиск на GPU
        cl::Buffer binary_data_buf(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                    binary_data_size * sizeof(int), const_cast<int *>(binary_data.data()));
        cl::Buffer binary_targets_buf(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                        binary_targets_size * sizeof(int), const_cast<int *>(binary_targets.data()));
        cl::Buffer binary_results_buf(context, CL_MEM_WRITE_ONLY,
                                        binary_targets_size * sizeof(int));

        auto binary_start = std::chrono::high_resolution_clock::now();

        binary_search_kernel.setArg(0, binary_data_buf);
        binary_search_kernel.setArg(1, binary_targets_buf);
        binary_search_kernel.setArg(2, binary_results_buf);
        binary_search_kernel.setArg(3, static_cast<int>(binary_data_size));
        binary_search_kernel.setArg(4, static_cast<int>(binary_targets_size));

        queue.enqueueNDRangeKernel(binary_search_kernel, cl::NullRange,
                                    cl::NDRange(binary_targets_size), cl::NullRange);
        queue.finish();

        std::vector<int> binary_results(binary_targets_size);
        queue.enqueueReadBuffer(binary_results_buf, CL_TRUE, 0,
                                binary_targets_size * sizeof(int), binary_results.data());

        auto binary_end = std::chrono::high_resolution_clock::now();
        results.binary_time = std::chrono::duration_cast<std::chrono::microseconds>(
                                    binary_end - binary_start)
                                    .count();
        results.binary_found = std::accumulate(binary_results.begin(),
                                                binary_results.end(), 0);

        return results;
    }
};

// Глобальный экземпляр GPU поисковика
GPUSearcher g_gpu_searcher;

// Инициализация hwloc
void initTopology()
{
    hwloc_topology_init(&topology);
    hwloc_topology_load(topology);
}

// Освобождение ресурсов hwloc
void cleanupTopology()
{
    hwloc_topology_destroy(topology);
}

// Установка аффинити к конкретному физическому ядру
void setAffinityToPhysicalCore(int physicalCoreId)
{
    int depth = hwloc_get_type_or_below_depth(topology, HWLOC_OBJ_CORE);
    hwloc_obj_t core = hwloc_get_obj_by_depth(topology, depth, physicalCoreId);

    if (core == NULL)
    {
        std::cerr << "Ошибка: физическое ядро " << physicalCoreId << " не найдено" << std::endl;
        return;
    }

    if (hwloc_set_cpubind(topology, core->cpuset, HWLOC_CPUBIND_THREAD) != 0)
    {
        std::cerr << "Ошибка установки аффинити к физическому ядру " << physicalCoreId << std::endl;
        char *str;
        hwloc_bitmap_asprintf(&str, core->cpuset);
        std::cerr << "CPUSet: " << str << std::endl;
        free(str);
    }
}

size_t linear_search(const std::vector<int> &data, int target)
{
    for (size_t i = 0; i < data.size(); ++i)
    {
        if (data[i] == target)
        {
            return i;
        }
    }
    return data.size();
}

size_t binary_search(const std::vector<int> &data, int target)
{
    size_t left = 0;
    size_t right = data.size();
    while (left < right)
    {
        size_t mid = left + (right - left) / 2;
        if (data[mid] == target)
        {
            return mid;
        }
        else if (data[mid] < target)
        {
            left = mid + 1;
        }
        else
        {
            right = mid;
        }
    }
    return data.size();
}

SearchResults search_sequential_timed(const std::vector<int> &linear_data, const std::vector<int> &binary_data,
                                      const std::vector<int> &linear_targets, const std::vector<int> &binary_targets)
{
    size_t linear_found = 0;
    size_t binary_found = 0;

    // Линейный поиск
    auto linear_start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < linear_targets.size(); ++i)
    {
        if (linear_search(linear_data, linear_targets[i]) != linear_data.size())
        {
            linear_found++;
        }
    }
    auto linear_end = std::chrono::high_resolution_clock::now();
    auto linear_time = std::chrono::duration_cast<std::chrono::microseconds>(linear_end - linear_start).count();

    // Бинарный поиск
    auto binary_start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < binary_targets.size(); ++i)
    {
        if (binary_search(binary_data, binary_targets[i]) != binary_data.size())
        {
            binary_found++;
        }
    }
    auto binary_end = std::chrono::high_resolution_clock::now();
    auto binary_time = std::chrono::duration_cast<std::chrono::microseconds>(binary_end - binary_start).count();

    return {static_cast<long double>(linear_time), static_cast<long double>(binary_time), linear_found, binary_found};
}

SearchResults search_openmp_timed(const std::vector<int> &linear_data, const std::vector<int> &binary_data,
                                  const std::vector<int> &linear_targets, const std::vector<int> &binary_targets)
{
    size_t linear_found = 0;
    size_t binary_found = 0;

    auto linear_start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for reduction(+ : linear_found)
    for (size_t i = 0; i < linear_targets.size(); ++i)
    {
        if (linear_search(linear_data, linear_targets[i]) != linear_data.size())
        {
            linear_found++;
        }
    }
    auto linear_end = std::chrono::high_resolution_clock::now();
    auto linear_time = std::chrono::duration_cast<std::chrono::microseconds>(linear_end - linear_start).count();

    auto binary_start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for reduction(+ : binary_found)
    for (size_t i = 0; i < binary_targets.size(); ++i)
    {
        if (binary_search(binary_data, binary_targets[i]) != binary_data.size())
        {
            binary_found++;
        }
    }
    auto binary_end = std::chrono::high_resolution_clock::now();
    auto binary_time = std::chrono::duration_cast<std::chrono::microseconds>(binary_end - binary_start).count();

    return {static_cast<long double>(linear_time), static_cast<long double>(binary_time), linear_found, binary_found};
}

SearchResults search_gpu_timed(const std::vector<int> &linear_data, const std::vector<int> &binary_data,
                               const std::vector<int> &linear_targets, const std::vector<int> &binary_targets)
{
    return g_gpu_searcher.search_gpu_timed(linear_data, binary_data, linear_targets, binary_targets);
}

SearchResults search_single_core_timed(const std::vector<int> &linear_data, const std::vector<int> &binary_data,
                                       const std::vector<int> &linear_targets, const std::vector<int> &binary_targets, int coreId)
{
    setAffinityToPhysicalCore(coreId);
    return search_sequential_timed(linear_data, binary_data, linear_targets, binary_targets);
}

template <typename TimeUnits>
SearchResults TestSearchTimed(SearchResults (*search_func)(const std::vector<int> &, const std::vector<int> &,
                                                           const std::vector<int> &, const std::vector<int> &),
                              const std::vector<int> &linear_data, const std::vector<int> &binary_data,
                              const std::vector<int> &linear_targets, const std::vector<int> &binary_targets,
                              int iterations)
{
    long double linear_sum = 0;
    long double binary_sum = 0;
    size_t linear_found = 0;
    size_t binary_found = 0;

    for (int i = 0; i < iterations; i++)
    {
        auto results = search_func(linear_data, binary_data, linear_targets, binary_targets);
        linear_sum += results.linear_time;
        binary_sum += results.binary_time;
        linear_found = results.linear_found;
        binary_found = results.binary_found;
    }

    return {linear_sum / iterations, binary_sum / iterations, linear_found, binary_found};
}

SearchResults TestSingleCoreTimed(SearchResults (*search_func)(const std::vector<int> &, const std::vector<int> &,
                                                               const std::vector<int> &, const std::vector<int> &),
                                  const std::vector<int> &linear_data, const std::vector<int> &binary_data,
                                  const std::vector<int> &linear_targets, const std::vector<int> &binary_targets,
                                  int iterations, int coreId)
{
    long double linear_sum = 0;
    long double binary_sum = 0;
    size_t linear_found = 0;
    size_t binary_found = 0;

    for (int i = 0; i < iterations; i++)
    {
        setAffinityToPhysicalCore(coreId);
        auto results = search_func(linear_data, binary_data, linear_targets, binary_targets);
        linear_sum += results.linear_time;
        binary_sum += results.binary_time;
        linear_found = results.linear_found;
        binary_found = results.binary_found;
    }

    return {linear_sum / iterations, binary_sum / iterations, linear_found, binary_found};
}

void printTopologyInfo()
{
    int depth = hwloc_get_type_or_below_depth(topology, HWLOC_OBJ_CORE);
    int num_cores = hwloc_get_nbobjs_by_depth(topology, depth);

    std::cout << "===============================================================================================\n";
    std::cout << "ИНФОРМАЦИЯ О ТОПОЛОГИИ ПРОЦЕССОРА\n";
    std::cout << "===============================================================================================\n";
    std::cout << "Обнаружено физических ядер: " << num_cores << std::endl;

    for (int i = 0; i < num_cores; i++)
    {
        hwloc_obj_t core = hwloc_get_obj_by_depth(topology, depth, i);
        char *cpuset_str;
        hwloc_bitmap_asprintf(&cpuset_str, core->cpuset);
        std::cout << "Физическое ядро " << i << ": логические CPU " << cpuset_str << std::endl;
        free(cpuset_str);
    }
    std::cout << "===============================================================================================\n\n";
}

void printResultsTable(int linear_targets_size, int binary_targets_size,
                       const SearchResults &seq_results, const SearchResults &omp_results,
                       const SearchResults &single_results, const SearchResults &gpu_results)
{
    double seq_ratio = (seq_results.binary_time > 0) ? seq_results.linear_time / seq_results.binary_time : 0;
    double omp_ratio = (omp_results.binary_time > 0) ? omp_results.linear_time / omp_results.binary_time : 0;
    double single_ratio = (single_results.binary_time > 0) ? single_results.linear_time / single_results.binary_time : 0;
    double gpu_ratio = (gpu_results.binary_time > 0) ? gpu_results.linear_time / gpu_results.binary_time : 0;

    std::cout << "===============================================================================================\n";
    std::cout << "СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ЛИНЕЙНОГО И БИНАРНОГО ПОИСКА\n";
    std::cout << "===============================================================================================\n";
    std::cout << "Размер данных для линейного поиска: " << LINEAR_DATA_SIZE << " элементов\n";
    std::cout << "Размер данных для бинарного поиска: " << BINARY_DATA_SIZE << " элементов\n";
    std::cout << "Количество целей для линейного поиска: " << linear_targets_size << "\n";
    std::cout << "Количество целей для бинарного поиска: " << binary_targets_size << "\n";
    std::cout << "Потоков OpenMP: " << omp_get_max_threads() << "\n";
    std::cout << "GPU доступен: " << (g_gpu_searcher.isAvailable() ? "Да" : "Нет") << "\n\n";

    std::cout << "===============================================================================================\n";
    std::cout << "ТАБЛИЦА ПРОИЗВОДИТЕЛЬНОСТИ (время в микросекундах)\n";
    std::cout << "===============================================================================================\n";
    std::cout << "-----------------------------------------------------------------------------------------------\n";
    std::cout << "| " << std::setw(13) << "Потоки" << " | "
              << std::setw(15) << "Линейный(мкс)" << " | "
              << std::setw(15) << "Бинарный(мкс)" << " | "
              << std::setw(10) << "Найдено_Л" << " | "
              << std::setw(10) << "Найдено_Б" << " |\n";
    std::cout << "-----------------------------------------------------------------------------------------------\n";

    std::cout << "| " << std::setw(7) << "1(seq)" << " | "
              << std::setw(15) << static_cast<long long>(seq_results.linear_time) << " | "
              << std::setw(15) << static_cast<long long>(seq_results.binary_time) << " | "
              << std::setw(10) << seq_results.linear_found << " | "
              << std::setw(10) << seq_results.binary_found << " |\n";

    std::cout << "| " << omp_get_max_threads() << "(omp)" << " | "
              << std::setw(15) << static_cast<long long>(omp_results.linear_time) << " | "
              << std::setw(15) << static_cast<long long>(omp_results.binary_time) << " | "
              << std::setw(10) << omp_results.linear_found << " | "
              << std::setw(10) << omp_results.binary_found << " |\n";

    std::cout << "| " << std::setw(6) << "1(core)" << " | "
              << std::setw(15) << static_cast<long long>(single_results.linear_time) << " | "
              << std::setw(15) << static_cast<long long>(single_results.binary_time) << " | "
              << std::setw(10) << single_results.linear_found << " | "
              << std::setw(10) << single_results.binary_found << " |\n";

    if (g_gpu_searcher.isAvailable())
    {
        std::cout << "| " << std::setw(6) << "GPU" << " | "
                  << std::setw(15) << static_cast<long long>(gpu_results.linear_time) << " | "
                  << std::setw(15) << static_cast<long long>(gpu_results.binary_time) << " | "
                  << std::setw(10) << gpu_results.linear_found << " | "
                  << std::setw(10) << gpu_results.binary_found << " |\n";
    }

    std::cout << "-----------------------------------------------------------------------------------------------\n\n";
}

// Сохранение в файл
void saveResultsToFile(const std::vector<std::tuple<int, int, SearchResults, SearchResults, SearchResults, SearchResults>> &results)
{
    std::ofstream file("search_results_with_openCL.csv");
    if (!file.is_open())
    {
        std::cerr << "Ошибка открытия файла для записи!" << std::endl;
        return;
    }

    file << "Линейные_Цели,Бинарные_Цели,Линейный_Посл,Бинарный_Посл,Линейный_OMP,Бинарный_OMP,Линейный_Ядро,Бинарный_Ядро,Линейный_GPU,Бинарный_GPU,"
         << "Отношение_Посл,Отношение_OMP,Отношение_Ядро,Отношение_GPU\n";

    for (const auto &result : results)
    {
        int linear_size, binary_size;
        SearchResults seq, omp, single, gpu;
        std::tie(linear_size, binary_size, seq, omp, single, gpu) = result;

        file << linear_size << "," << binary_size << ","
             << std::fixed << std::setprecision(2) << seq.linear_time << ","
             << std::fixed << std::setprecision(2) << seq.binary_time << ","
             << std::fixed << std::setprecision(2) << omp.linear_time << ","
             << std::fixed << std::setprecision(2) << omp.binary_time << ","
             << std::fixed << std::setprecision(2) << single.linear_time << ","
             << std::fixed << std::setprecision(2) << single.binary_time << ","
             << std::fixed << std::setprecision(2) << gpu.linear_time << ","
             << std::fixed << std::setprecision(2) << gpu.binary_time << ","
             << std::fixed << std::setprecision(2) << (seq.binary_time > 0 ? seq.linear_time / seq.binary_time : 0) << ","
             << std::fixed << std::setprecision(2) << (omp.binary_time > 0 ? omp.linear_time / omp.binary_time : 0) << ","
             << std::fixed << std::setprecision(2) << (single.binary_time > 0 ? single.linear_time / single.binary_time : 0) << ","
             << std::fixed << std::setprecision(2) << (gpu.binary_time > 0 ? gpu.linear_time / gpu.binary_time : 0) << "\n";
    }
    file.close();
}

void generateTestData(std::vector<int> &linear_data, std::vector<int> &binary_data,
                      std::vector<int> &all_linear_targets, std::vector<int> &all_binary_targets)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, BINARY_DATA_SIZE * 10);

    std::cout << "Генерация данных для линейного поиска (" << LINEAR_DATA_SIZE << " элементов)...\n";
    linear_data.resize(LINEAR_DATA_SIZE);
    for (size_t i = 0; i < LINEAR_DATA_SIZE; ++i)
    {
        linear_data[i] = dist(gen);
    }

    std::cout << "Генерация данных для бинарного поиска (" << BINARY_DATA_SIZE << " элементов)...\n";
    binary_data.resize(BINARY_DATA_SIZE);
    for (size_t i = 0; i < BINARY_DATA_SIZE; ++i)
    {
        binary_data[i] = dist(gen);
    }
    std::sort(binary_data.begin(), binary_data.end());

    int max_linear_targets = 50000;
    int max_binary_targets = 500000;

    std::cout << "Генерация целей для линейного поиска (" << max_linear_targets << " целей)...\n";
    all_linear_targets.resize(max_linear_targets);
    for (size_t i = 0; i < max_linear_targets; ++i)
    {
        if (i % 2 == 0)
        {
            all_linear_targets[i] = linear_data[dist(gen) % LINEAR_DATA_SIZE];
        }
        else
        {
            all_linear_targets[i] = dist(gen);
        }
    }

    std::cout << "Генерация целей для бинарного поиска (" << max_binary_targets << " целей)...\n";
    all_binary_targets.resize(max_binary_targets);
    for (size_t i = 0; i < max_binary_targets; ++i)
    {
        if (i % 2 == 0)
        {
            all_binary_targets[i] = binary_data[dist(gen) % BINARY_DATA_SIZE];
        }
        else
        {
            all_binary_targets[i] = dist(gen);
        }
    }

    std::cout << "Генерация данных завершена!\n\n";
}

int main()
{
    initTopology();
    printTopologyInfo();

    int max_threads = omp_get_max_threads();
    omp_set_num_threads(max_threads);

    std::vector<int> linear_data, binary_data, all_linear_targets, all_binary_targets;
    generateTestData(linear_data, binary_data, all_linear_targets, all_binary_targets);

    // Прогрессивные размеры тестов
    std::vector<int> linear_test_sizes = {100, 500, 1000, 2000, 5000, 10000, 25000, 50000};
    std::vector<int> binary_test_sizes = {1000, 5000, 10000, 20000, 50000, 100000, 250000, 500000};

    std::vector<std::tuple<int, int, SearchResults, SearchResults, SearchResults, SearchResults>> results;

    for (size_t i = 0; i < linear_test_sizes.size(); ++i)
    {
        int linear_size = linear_test_sizes[i];
        int binary_size = binary_test_sizes[i];

        std::vector<int> linear_targets(all_linear_targets.begin(), all_linear_targets.begin() + linear_size);
        std::vector<int> binary_targets(all_binary_targets.begin(), all_binary_targets.begin() + binary_size);

        std::cout << "Тестирование с " << linear_size << " целями для линейного поиска и "
                  << binary_size << " целями для бинарного поиска...\n";

        auto seq_results = TestSearchTimed<std::chrono::microseconds>(
            search_sequential_timed, linear_data, binary_data, linear_targets, binary_targets, ITERATIONS);

        auto omp_results = TestSearchTimed<std::chrono::microseconds>(
            search_openmp_timed, linear_data, binary_data, linear_targets, binary_targets, ITERATIONS);

        auto single_results = TestSingleCoreTimed(
            search_sequential_timed, linear_data, binary_data, linear_targets, binary_targets, ITERATIONS, 0);

        auto gpu_results = TestSearchTimed<std::chrono::microseconds>(
            search_gpu_timed, linear_data, binary_data, linear_targets, binary_targets, ITERATIONS);

        results.emplace_back(linear_size, binary_size, seq_results, omp_results, single_results, gpu_results);
        printResultsTable(linear_size, binary_size, seq_results, omp_results, single_results, gpu_results);
    }

    saveResultsToFile(results);
    std::cout << "Результаты сохранены в search_results_with_openCL.csv" << std::endl;

    cleanupTopology();

    return 0;
}

// g++ -std=c++17 -lhwloc -fopenmp -lOpenCL searchOpenCl.cpp -o searchOpenCl && ./searchOpenCl

// check smt:
// cat /sys/devices/system/cpu/smt/active

// off smt:
// echo off | sudo tee /sys/devices/system/cpu/smt/control

// on smt:
// echo on | sudo tee /sys/devices/system/cpu/smt/control