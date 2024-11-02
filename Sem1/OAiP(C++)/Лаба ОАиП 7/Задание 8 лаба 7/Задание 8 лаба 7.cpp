#include <iostream>

void translate_num(int n) {
    short arr[20], i = 0;

    for (; n > 0; i++) {
        arr[i] = n % 3;
        n = n / 3;
    }

    for (bool nylik = 1; nylik;) {
        nylik = 0;
        for (short j = i - 1; j > 0; j--) {
            if (arr[j - 1] == 0) {
                arr[j]--;
                arr[j - 1] = 3;
                nylik = 1;
            }
        }
    }

    for (short j = i - 1; j >= 0; j--) {
        if (arr[j] < 1 && j == i - 1) continue;
        std::cout << arr[j];
    }
}

int main() {
    int n;
    std::cin >> n;

    translate_num(n);

    return 0;
}