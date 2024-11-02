#include <iostream>
#include <string>

long long Factorial(long long L_string) {
    if (L_string == 0) return 1;
    long long factorial = 1;
    if (L_string == 1) return factorial;
    return L_string * Factorial(L_string - 1);
}

long long Replay(std::string& anagramm, long long L_string) {
    long long mult = 1;
    for (char i = 'A'; i <= 'z'; i++) {
        int replay = 0;
        for (int p = 0; p < L_string; p++) {
            if (anagramm[p] == i) { replay++; }
        }
        mult *= Factorial(replay);
    }
    return mult;
}

int main() {
    std::string anagramm;
    long long L_string;
    std::cin >> anagramm;
    L_string = anagramm.length();
    std::cout << Factorial(L_string) / Replay(anagramm, L_string);
    return 0;
}