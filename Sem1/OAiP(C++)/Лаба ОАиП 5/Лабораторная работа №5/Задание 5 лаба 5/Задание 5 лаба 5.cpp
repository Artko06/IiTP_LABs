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
int** arr_creation(int** arr_A, int** arr_B, const int str, const int stlb) {
    for (int q = 0, ib = 0; q < str; q++) {
        for (int ia = 0; ia < stlb; ia++) {
            if (arr_A[q][ia] % 2 != 0 && ia % 2 == 0) {
                arr_B[ib][0] = arr_A[q][ia];
                ib++;
            }
        }
    }
        return arr_B;
}
int average(int** arr, const int str, const int stlb) {
    double average = 0;
    for (int q = 0; q < str; q++) {
        for (int i = 0; i < stlb; i++) {
            average += arr[q][i];
        }
    }
    average /= str;
    return average;
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
        std::cout << "Создать динамический массив из элементов, расположенных в четных столбцах данного массива и имеющих нечетное значение. Вычислить среднее арифметическое элементов динамического массива\n";
    short n_odd = 0;
    std::string stroka_str, stroka_stlb;
    int str = 0, stlb = 0;
    std::cout << "Введите кол-во строк: ";
    std::cin >> stroka_str;
    correctInput_N(stroka_str, str);
    if (str == 0) { return 0; }
    std::cout << "Введите кол-во столбцов: ";
    std::cin >> stroka_stlb;
    correctInput_N(stroka_stlb, stlb);
    if (stlb == 0) { return 0; }
    int** arr_A, ** arr_B;
    arr_A = arr_generator(str, stlb);
    arr_generatorNum(arr_A, str, stlb);
    std::cout << "Исходный массив:\n";
    arr_input2(arr_A, str, stlb);
    for (int q = 0; q < str; q++) {
        for (int i = 0; i < stlb; i++) {
            if (arr_A[q][i] % 2 != 0 && i % 2 == 0) {
                n_odd++;
            }
        }
    }
    if (n_odd == 0) {
        std::cout << "Ни один элемент массива не удовлетворяет условию, ПЕРЕЗАПУСК\n";
        return main();
    }
    arr_B = arr_generator(n_odd, 1);
    arr_creation(arr_A, arr_B, str, stlb);
    std::cout << "Полученный массив:\n";
    arr_input1(arr_B, n_odd, 1);
    std::cout << "\nСреднее арифметическое нового массива:\n";
    if (n_odd) { std::cout << average(arr_B, n_odd, 1) << '\n'; }
    arr_destroyer(arr_A, stlb);
    arr_destroyer(arr_B, 1);
    std::cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    std::cin >> c;
    }
    return 0;
}