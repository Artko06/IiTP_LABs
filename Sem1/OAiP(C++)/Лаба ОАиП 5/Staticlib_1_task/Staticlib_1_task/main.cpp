#include <iostream>
int** arr_generator(const int N) {
    int** arr = new int* [N];
    for (int i = 0; i < N; i++) {
        arr[i] = new int[N];
    }
    return arr;
}
int** arr_generatorNum(int** arr, const int N) {
    for (int q = 0; q < N; q++) {
        for (int i = 0; i < N; i++) {
            std::cout << q + 1 << " строка " << i + 1 << " cтолбец:\n";
            while (true) {
                if (!(std::cin >> arr[q][i])) {
                    std::cin.clear();
                    while (std::cin.get() != '\n');
                    std::cout << "Неверный ввод \n";
                    continue;
                }
                if (std::cin.peek() != '\n') {
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Неверный ввод \n";
                    continue;
                }
                break;
            }
        }
    }
    return arr;
}
int** arr_input(int** arr, const int N) {
    for (int q = 0; q < N; q++) {
        for (int i = 0; i < N; i++) {
            std::cout << arr[q][i] << '\t';
        }
        std::cout << '\n';
    }
    return arr;
}
int** arr_compare(int** arr_A, int** arr_B, int** arr_C, int N) {
    for (int q = 0; q < N; q++) {
        for (int i = 0; i < N; i++) {
            if (abs(arr_B[q][i]) > abs(arr_C[q][i])) {
                arr_A[q][i] = arr_B[q][i];
            }
            else {
                arr_A[q][i] = arr_C[q][i];
            }
        }
    }
    return arr_A;
}
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
void context() {
    std::cout << "Составить программу вычисления элементов двумерного динамического массива - матрицы A размерностью n.Значения элементов aij определить согласно выражениям\n";
}
void arr_destroyer(int** arr, int N) {
    for (int i = 0; i < N; i++) {
        delete[] arr[i];
        arr[i] = nullptr;
    }
    delete[] arr;
    arr = nullptr;
}
