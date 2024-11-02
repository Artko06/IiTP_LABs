#include <iostream>

int main() {
    short t;
    std::cin >> t;

    for (int i = 0; i < t; i++) {
        long long num;
        std::cin >> num;

        std::cout << (num + 1) / 10 << '\n';
    }
    return 0;
}