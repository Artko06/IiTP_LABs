#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>
#include <cuda_runtime.h>

using namespace std;
using namespace std::chrono;

__global__ void solveSKernel(double x, double epsilon, double *result, int *converged, int maxIterations)
{
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx >= maxIterations || *converged <= idx)
        return; // Прерываем выполнение, если достигнута сходимость

    double base = (x - 1) / (x + 1);
    double term = 1.0 / (2 * idx + 1) * pow(base, 2 * idx + 1);

    // Атомарное сложение для накопления результата
    atomicAdd(result, term);

    // Проверяем условие сходимости
    if (fabs(term) < epsilon)
    {
        atomicMin(converged, idx + 1); // Обновляем минимальное количество итераций
    }
}

double solveS(double x, double epsilon, int &iterations)
{
    const int maxIterations = 100000; // Ограничение на количество итераций
    double result = 0.0;
    int converged = maxIterations;

    // Выделяем память на устройстве
    double *d_result;
    int *d_converged;
    cudaMalloc((void **)&d_result, sizeof(double));
    cudaMalloc((void **)&d_converged, sizeof(int));

    // Инициализируем значения на устройстве
    cudaMemset(d_result, 0, sizeof(double));
    cudaMemcpy(d_converged, &converged, sizeof(int), cudaMemcpyHostToDevice);

    // Конфигурация CUDA
    int threadsPerBlock = 256;
    int blocksPerGrid = (maxIterations + threadsPerBlock - 1) / threadsPerBlock;

    // Запуск CUDA kernel
    solveSKernel<<<blocksPerGrid, threadsPerBlock>>>(x, epsilon, d_result, d_converged, maxIterations);

    // Синхронизация устройства
    cudaDeviceSynchronize();

    // Копируем результат обратно на хост
    cudaMemcpy(&result, d_result, sizeof(double), cudaMemcpyDeviceToHost);
    cudaMemcpy(&converged, d_converged, sizeof(int), cudaMemcpyDeviceToHost);

    // Освобождаем память на устройстве
    cudaFree(d_result);
    cudaFree(d_converged);

    // Устанавливаем количество итераций
    iterations = converged;
    return result;
}

// Вычисление Y(x) (не требует распараллеливания)
double solveY(double x)
{
    return log(x) / 2;
}

int main()
{
    double a, b, h, epsilon;

    cout << "Введите a: ";
    cin >> a;
    cout << "Введите b: ";
    cin >> b;
    cout << "Введите шаг h: ";
    cin >> h;
    cout << "Введите epsilon: ";
    cin >> epsilon;

    ofstream outputFile("results.txt");
    if (!outputFile)
    {
        cerr << "Ошибка открытия файла!" << endl;
        return 1;
    }

    cout << " x | Y(x) | S(x) | Итерации | Время (сек) " << endl;
    cout << "-------------------------------------------------" << endl;

    for (double x = a; x <= b; x += h)
    {
        if (x <= 0)
            continue;
        if (h <= 0 && epsilon <= 0)
            break;

        int iterations = 0;
        double resultY = solveY(x);

        auto start = high_resolution_clock::now();
        double resultS = solveS(x, epsilon, iterations);
        auto end = high_resolution_clock::now();

        duration<double> elapsedTime = end - start;

        cout << x << " | " << resultY << " | " << resultS << " | " << iterations << " | " << elapsedTime.count() << endl;
        outputFile << iterations << " | " << elapsedTime.count() << endl;
    }

    outputFile.close();
    return 0;
}