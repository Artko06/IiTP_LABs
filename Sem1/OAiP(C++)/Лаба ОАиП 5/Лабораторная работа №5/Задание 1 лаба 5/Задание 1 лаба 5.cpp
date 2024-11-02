#include <iostream>
#include "..\..\Staticlib_1_task\Staticlib_1_task\Header.h"
int main() {
    setlocale(LC_ALL, "RU");
    char c = '+';
    while (c == '+') {
    context();
    std::string stroka;
    int N = 0;
    std::cout << "Введите размер матриц: ";
    std::cin >> stroka;
    correctInput_N(stroka, N);
    if (N == 0) { return 0; }
    int** arr_A, ** arr_B, ** arr_C;
    arr_A = arr_generator(N);
    arr_B = arr_generator(N);
    std::cout << "Заполнение матрицы B:\n";
    arr_generatorNum(arr_B, N);
    arr_C = arr_generator(N);
    std::cout << "Заполнение матрицы C:\n";
    arr_generatorNum(arr_C, N);
    std::cout << "Вывод матрицы B:\n";
    arr_input(arr_B, N);
    std::cout << "Вывод матрицы C:\n";
    arr_input(arr_C, N);

    arr_compare(arr_A, arr_B, arr_C, N);
    std::cout << "Вывод матрицы A:\n";
    arr_input(arr_A, N);

    arr_destroyer(arr_A, N);
    arr_destroyer(arr_B, N);
    arr_destroyer(arr_C, N);
    std::cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    std::cin >> c;
    }
    return 0;
}
