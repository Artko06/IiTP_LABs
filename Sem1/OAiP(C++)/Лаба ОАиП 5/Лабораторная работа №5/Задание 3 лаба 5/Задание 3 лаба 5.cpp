#include <iostream>
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
int** arr_generator(const int str, const int stlb) {
    int** arr = new int* [str];
    for (int i = 0; i < str; i++) {
        arr[i] = new int[stlb];
    }
    return arr;
}
int** arr_generatorNum(int** arr, const int str, const int stlb) {
    for (int q = 0; q < str; q++) {
        for (int i = 0; i < stlb; i++) {
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
int** arr_input2(int** arr, const int str, const int stlb) {
    for (int q = 0; q < str; q++) {
        for (int i = 0; i < stlb; i++) {
            std::cout << arr[q][i] << '\t';
        }
        std::cout << '\n';
    }
    return arr;
}
int** arr_input1(int** arr, const int str, const int stlb) {
    for (int q = 0; q < str; q++) {
        for (int i = 0; i < stlb; i++) {
            std::cout << arr[q][i];
        }
        std::cout << '\t';
    }
    return arr;
}
int** arr_creation(int** arr_A, int** arr_B, const int str) {
    for (int i = 0, q = 0; i < str; i++) {
        if (arr_A[i][i] % 2 == 0) {
            arr_B[q][0] = arr_A[i][i];
            q++;
        }
    }
    return arr_B;
}

void arr_multiplication(int** arr_B, const int str, const int stlb) {
    long long mult = 1;
    for (int q = 0; q < str; q++) {
        for (int i = 0; i < stlb; i++) {
            mult *= arr_B[q][i];
        }
    }
    if (mult == 1) {
        std::cout << "Все элементы на главной диагонали нечётные\n";
    }
    else {
        std::cout << "\nПроизведние элементов главной диагонали:\n" << mult << '\n';
    }
}
void arr_destroyer(int** arr, int stlb) {
    for (int i = 0; i < stlb; i++) {
        delete[] arr[i];
        arr[i] = nullptr;
    }
    delete[] arr;
    arr = nullptr;
}
int main() {
    setlocale(LC_ALL, "RU");
    char c = '+';
    while (c == '+') {
    std::cout << " Создать динамический массив из элементов, расположенных на главной диагонали матрицы и имеющих чётное значение. Вычислить произведение элементов динамического массива\n";
    std::string stroka;
    int str = 0, stlb;
    short n_even = 0;
    std::cout << "Введите размер массива: ";
    std::cin >> stroka;
    correctInput_N(stroka, str);
    if (str == 0) { return 0; }
    stlb = str;
    int** arr_A, ** arr_B;
    arr_A = arr_generator(str, stlb);
    arr_generatorNum(arr_A, str, stlb);
    std::cout << "Исходная матрица:\n";
    arr_input2(arr_A, str, stlb);
    for (int i = 0; i < str; i++) {
        if (arr_A[i][i] % 2 == 0) n_even++;
    }
    arr_B = arr_generator(n_even, 1);
    arr_creation(arr_A, arr_B, str);
    std::cout << "Полученый массив:\n";
    arr_input1(arr_B, n_even, 1);
    arr_multiplication(arr_B, n_even, 1);
    arr_destroyer(arr_A, stlb);
    arr_destroyer(arr_B, 1);
    std::cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    std::cin >> c;
    }
    return 0;
}