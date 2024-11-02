#include <iostream>

int main() {
    short t;
    std::cin >> t;

    for (int iter = 0; iter < t; iter++) {
        bool yes = 0;
        std::string str1, str2, str3;
        std::cin >> str1 >> str2;
        str3 = str1;

        for (int i = 0; i < str1.size() / 2; i++) {
            std::swap(str1[i], str1[str1.size() - i - 1]);
        }

        str1.erase(0, 1);
        str3 = str3 + str1;

        while (str3.size() >= str2.size()) {
            if (str3.find(str2) != std::string::npos) {
                yes = 1;
            }
            str3.erase(str3.size() / 2, 2);
        }

        (yes)? std::cout << "YES\n" : std::cout << "NO\n";
    }
    return 0;
}