#include <iostream>

int main() {
    std::string strochenka;

    std::cin >> strochenka;
    /*
    if (strochenka.size() == 1) {
        std::cout << 1;
        return 0;
    }
    */
    for (int i = 0; i < strochenka.size() / 2; i++) {
        if (strochenka[i] != strochenka[strochenka.size() - 1 - i]) {
            std::cout << strochenka.size();
            return 0;
        }
    }

    for (int i = 0; i < strochenka.size(); i++) {
        if (strochenka[i] == strochenka[strochenka.size() - 1 - i] && strochenka[strochenka.size() / 2] == strochenka[0]);
        else{ 
            std::cout << strochenka.size() - 1;
            return 0;
        }
    }

    std::cout << -1;
    return 0;
}