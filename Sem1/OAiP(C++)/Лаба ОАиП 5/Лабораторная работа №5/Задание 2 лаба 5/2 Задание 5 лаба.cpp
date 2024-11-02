#include <iostream>
#include <Windows.h>
int main();

int correctInput_N(std::string& stroka, int& new_Num) {
    if (stroka.length() > 9) {
        std::cout << "Некоректный ввод\n";
        return main();
    }
    else if (stroka.length() == 1 && stroka[0] == 0) {
        std::cout << "Некоректный ввод\n";
        return main();
    }
    for (int i = 0; i < stroka.length(); i++) {
        if (stroka[i] >= '0' && stroka[i] <= '9') {
            if (i == stroka.length() - 1) {
                for (int i = stroka.length() - 1, iter = 1; i >= 0, iter < pow(10, stroka.length()); i--, iter *= 10) {
                    new_Num += (stroka[i] - 48) * iter;
                }
                return new_Num;
            }
        }
        else {
            std::cout << "Некоректный ввод\n";
            return main();
        }
    }
}


int main() {
    setlocale(LC_ALL, "RU");
    char c = '+';
    while (c == '+') {
    typedef int* (*Arr_generator)(const int);
    Arr_generator arr_generator = (Arr_generator)GetProcAddress(LoadLibrary(L"DynamicLib.dll"), "arr_generator");

    typedef int* (*Arr_generatorNum)(int*, const int);
    Arr_generatorNum arr_generatorNum = (Arr_generatorNum)GetProcAddress(LoadLibrary(L"DynamicLib.dll"), "arr_generatorNum");

    typedef int* (*Arr_input)(int*, const int);
    Arr_input arr_input = (Arr_input)GetProcAddress(LoadLibrary(L"DynamicLib.dll"), "arr_input");

    typedef void (*Arr_destroyer)(int*, int);
    Arr_destroyer arr_destroyer = (Arr_destroyer)GetProcAddress(LoadLibrary(L"DynamicLib.dll"), "arr_destroyer");

    typedef int* (*Recursion)(int*, int);
    Recursion recursion = (Recursion)GetProcAddress(LoadLibrary(L"DynamicLib.dll"), "recursion");

    typedef void (*Context)();
    Context context = (Context)GetProcAddress(LoadLibrary(L"DynamicLib.dll"), "context");
    
    context();
    std::string stroka;
    int N = 0;
    std::cout << "Кол-во элементов массива: ";
    std::cin >> stroka;
    correctInput_N(stroka, N);
    if (N == 0) { return 0; }
    int* arr_A = arr_generator(N);
    arr_generatorNum(arr_A, N);
    arr_input(arr_A, N);
    recursion(arr_A, N);
    arr_destroyer(arr_A, N);
    std::cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    std::cin >> c;
    }
    return 0;
}