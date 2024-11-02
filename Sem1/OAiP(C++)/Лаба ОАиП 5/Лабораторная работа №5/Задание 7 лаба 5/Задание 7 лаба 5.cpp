#include <iostream>
long long Exponentiation(long long k, long long n, long long t) {
    long long ans = 1;
    while (n > 0) {
        long long last_bit = (n & 1);
        if (last_bit) {
            ans = (ans * k) % t;
        }
        k = (k * k) % t;

        n = n >> 1;
    }
    return ans;
}
int main() {
    setlocale(LC_ALL, "RU");
    long long k, n, t;
    long long* answers = NULL;
    short size_answers = 0;
    while (true) {
        std::cout << "Введите k, n, t соответвенно:\n";
        std::cin >> k >> n >> t;
        long long T = 1;
        for (int i = 0; i < t; i++) {
            T = T * 10;
        }
        n %= T;
        k %= T;
        if (k != 0 && n != 0 && t != 0) {
            size_answers++;
            answers = (long long*)realloc(answers, size_answers * sizeof(long long));
            answers[size_answers - 1] = 0;
            answers[size_answers - 1] += Exponentiation(k, n, T);
        }
        else { break; }
    }
    std::cout << "Полученные значения:\n";
    for (int i = 0; i < size_answers; i++) {
        std::cout << "Case #" << i + 1 << ": " << answers[i] << '\n';
    }
    free(answers);
    answers = NULL;
    return 0;
}