#include <iostream>
int recursion(int p) {
    if (p == 0) { return 0; }
    else if (p % 10 != 0) { return p % 10; }
    else { return recursion(p / 10); }
}

int main() {
    setlocale(LC_ALL, "RU");
    int p, q;
    short size_answers = 0;
    int* answers = NULL;
    while (true) {
        std::cout << "Введите p и q соответсвенно: ";
        std::cin >> p >> q;
        if (q >= 0) {
            size_answers++;
            answers = (int*)realloc(answers, size_answers * sizeof(int));
            answers[size_answers - 1] = 0;
        }
        else { break; }
        while (p <= q) {
              answers[size_answers - 1] += recursion(p);
              p++;
        }
    }
    std::cout << "Полученные значения:\n";
    for (int i = 0; i < size_answers; i++) {
        std::cout << answers[i] << '\n';
    }
    free(answers);
    answers = NULL;
    return 0;
}