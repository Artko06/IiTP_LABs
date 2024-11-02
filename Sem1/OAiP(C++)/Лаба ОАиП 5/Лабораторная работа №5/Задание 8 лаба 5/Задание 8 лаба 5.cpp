#include <iostream>
long long* arr_generator(const short N) {
    long long* arr = new long long [N];
    return arr;
}
long long* arr_generatorNum(long long* arr, const short N_test) {
    for (short i = 0; i < N_test; i++) {
        std::cout << "Введите " << i + 1 << " число:\n";
        std::cin >> arr[i];

    }
    return arr;
}
long long recurcion(long long N_arr) {
    return (N_arr + 1) * (N_arr + 1) / 4;
}
int main() {
    setlocale(LC_ALL, "RU");
    short N_test;
    long long N;
    std::cout << "Введите кол-во тестов: ";
    std::cin >> N_test;
    long long* arr_A, * arr_sum;
    arr_A = arr_generator(N_test);
    arr_generatorNum(arr_A, N_test);
    arr_sum = arr_generator(N_test);
    for (short i = 0; i < N_test; i++) {
        arr_sum[i] = 0;
    }
    for (short i = 0; i < N_test; i++) {
        if (arr_A[i] % 2 != 0) {
            for (long long q = arr_A[i]; q > 0; q /= 2) {
                if (q % 2 != 0) { arr_sum[i] += recurcion(q); }
                else { arr_sum[i] += recurcion(q - 1); }
                if (q % 2 != 0) { q--; }
            }
        }
        else {
            for (long long q = arr_A[i]; q > 0; q /= 2) {
                if (q % 2 != 0) { arr_sum[i] += recurcion(q); }
                else { arr_sum[i] += recurcion(q - 1); }
            }
        }
    }
    for (short i = 0; i < N_test; i++) {
        std::cout << arr_sum[i] << '\n';
    }
    return 0;
}