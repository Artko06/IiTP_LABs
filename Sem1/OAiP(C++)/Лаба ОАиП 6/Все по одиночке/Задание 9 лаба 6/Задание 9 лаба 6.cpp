#include <iostream>
#include <cmath>
#include <string>

long long Exponentiation(long long n, int m, std::string& s1, std::string& s2) {
    long long ans = 1;
    long long k = 26;
    while (n > 0) {
        bool last_bit = (n & 1);
        if (last_bit) {
            ans = (ans * k) % m;
        }
        k = (k * k) % m;

        n = n >> 1;
    }
    if (s1 == s2) { return ans % m; }
    return (2 * ans) % m ;
}

long long Exponentiation(long long n, int m) {
    long long ans = 1;
    long long k = 26;
    while (n > 0) {
        bool last_bit = (n & 1);
        if (last_bit) {
            ans = (ans * k) ;
        }
        k = (k * k);

        n = n >> 1;
    }
    return ans;
}

long long subtraction(std::string &s1, std::string &s2, long long &L, long long &n, const int &m) {
    if (s1.size() > s2.size()) {
        if (s1.compare(0, s2.size(), s2) == 0 && s1.compare((int)s1.size() - (int)s2.size(), s2.size(), s2) == 0 && L - 2 * (int)s1.size() >= 0) {
            return Exponentiation(L - 2 * (int)s1.size(), m);
        }
        else if (L - 2 * (int)s1.size() < 0) {
            if (s1.compare(0, (int)s1.size() - (int)s2.size() - abs(n), s1, (int)s1.size() - ((int)s1.size() - (int)s2.size() - abs(n))) == 0) { return 1; }
        }
    }
    else if (s2.size() > s1.size()) {
        if (s2.compare(0, s1.size(), s1) == 0 && s2.compare((int)s2.size() - (int)s1.size(), s1.size(), s1) == 0 && L - 2 * (int)s2.size() >= 0) {
            return Exponentiation(L - 2 * (int)s2.size(), m);
        }
        else if (L - 2 * (int)s2.size() < 0)  {
            if (s2.compare(0, (int)s2.size() - (int)s1.size() - abs(n), s2, (int)s2.size() - ((int)s2.size() - (int)s1.size() - abs(n))) == 0) { return 1; }
        }
    }
    return 0;
}

int main() {
    short t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        int m;
        long long L, n;
        std::string s1, s2;

        std::cin >> L >> m >> s1 >> s2;
        n = L - (int)s1.size() - (int)s2.size();

        if (n >= 0 && s1.size() <= L && s2.size() <= L) {
            if (s1 == s2) { std::cout << Exponentiation(n, m, s1, s2) << '\n'; }

            else { std::cout << Exponentiation(n, m, s1, s2) - subtraction(s1, s2, L, n, m) << '\n'; }
        }
        else{
            if (s1.size() <= L && s2.size() <= L && s1.compare(0, abs(n), s2, (int)s2.size() - abs(n), abs(n)) == 0
                && s2.compare(0, abs(n), s1, (int)s1.size() - abs(n), abs(n)) == 0
                && s1.substr(0,(int)s1.size() - abs(n)) + s2 != s2 + s1.substr(abs(n))) { std::cout << 2 % m << '\n'; }

            else if (s1.size() <= L && s2.size() <= L && s1.compare(0, abs(n), s2, (int)s2.size() - abs(n), abs(n)) == 0
                || s2.compare(0, abs(n), s1, (int)s1.size() - abs(n), abs(n)) == 0) { std::cout << 1 % m << '\n'; }

            else { std::cout << 0 << '\n'; }
        }
    }
    return 0;
}