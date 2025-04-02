#include <iostream>
#include <fstream>
#include <chrono>
#include <cmath>

using namespace std;
using namespace std::chrono;


double solveS(double x, double epsilon, int &iterations)
{
    double result = 0.0;
    double term;
    double base = (x - 1) / (x + 1);
    double base_pow = base;
    iterations = 0;

    while (true)
    {
        term = 1.0 / (2 * iterations + 1) * base_pow;
        ++iterations;

        if (fabs(term) < epsilon)
            break;

        // Ассемблерная вставка для сложения (добавление term к result)
        asm volatile(
            "fldl %1;"  // Загружаем term в FPU стек
            "faddl %0;" // Складываем с result (верх стека + result)
            "fstpl %0;" // Записываем обратно в result
            : "=m"(result)
            : "m"(term));

        base_pow *= base * base; // (2i+1..2i+3..2i+5,,2i+7..)
    }
    
    return result;
}

double solveY(double x){
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
        if(x <= 0) continue;
        if(h <= 0 && epsilon <= 0) break;

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
