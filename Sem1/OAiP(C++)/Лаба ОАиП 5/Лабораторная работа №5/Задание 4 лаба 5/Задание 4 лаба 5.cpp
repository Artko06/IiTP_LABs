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
double** arr_generator(const int str, const int stlb) {
    double** arr = new double* [str];
    for (int i = 0; i < str; i++) {
        arr[i] = new double[stlb];
    }
    return arr;
}
double** arr_generatorNum(double** arr, const int str, const int stlb) {
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
double** arr_input(double** arr, const int str, const int stlb) {
    for (int q = 0; q < str; q++) {
        for (int i = 0; i < stlb; i++) {
            std::cout << arr[q][i] << '\t';
        }
        std::cout << '\n';
    }
    return arr;
}
int check_NULL(double** arr_A, const int str, const int stlb) {
    short N_null = 0;
    std::cout << "Расположение нулей в массиве:\n";
    for (int q = 0; q < str; q++) {
        for (int i = 0; i < stlb; i++) {
            if (arr_A[q][i] == 0) {
                N_null++;
                std::cout << N_null << ") " << q << ' ' << i << '\n';
            }
        }
    }
    std::cout << "Кол-во нулей в массиве: ";
    return N_null;
}
double** arr_input_inverce(double** arr, const int str, const int stlb) {
    for (int q = str - 1; q >= 0; q--) {
        for (int i = stlb - 1; i >= 0; i--) {
            std::cout << arr[q][i] << '\t';
        }
        std::cout << '\n';
    }
    return arr;
}
void arr_destroyer(double** arr, int stlb) {
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
        std::cout << "Создать двумерный динамический массив вещественных чисел. Определить, встречаются ли среди них элементы с нулевым значением. Если встречаются такие элементы, то определить их индексы и общее количество. Переставить элементы этого массива в обратном порядке и вывести на экран\n";
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
    double** arr_A;
    arr_A = arr_generator(str, stlb);
    arr_generatorNum(arr_A, str, stlb);
    std::cout << "Массив:\n";
    arr_input(arr_A, str, stlb);
    std::cout << check_NULL(arr_A, str, stlb) << '\n';
    std::cout << "Элементы этого массива в обратном порядке:\n";
    arr_input_inverce(arr_A, str, stlb);
    arr_destroyer(arr_A, stlb);
    std::cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    std::cin >> c;
    }
    return 0;
}
