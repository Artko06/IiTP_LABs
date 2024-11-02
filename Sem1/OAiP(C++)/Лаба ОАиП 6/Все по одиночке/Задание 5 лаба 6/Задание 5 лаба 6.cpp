#include <cmath>
#include <iostream>

bool check_first_last(std::string s1, std::string s2, long long L) {
    if (s1.size() > s2.size()) {
        for (int i = 0, i2 = s1.size() - 1; i < s2.size(); i++, i2--) {
            if (s1[i] != s2[i] || s1[i2] != s2[s2.size() - 1 - i]) { break; }
            else if (i == s2.size() - 1) { return 1; }
        }
    }
    else if (s2.size() > s1.size()) {
        for (int i = 0, i2 = s2.size() - 1; i < s1.size(); i++, i2--) {
            if (s1[i] != s2[i] || s2[i2] != s1[s1.size() - 1 - i]) { break; }
            else if (i == s1.size() - 1) { return 1; }
        }
    }
    return 0;
}

bool one_symbol(std::string s1, std::string s2, long long L) {
    for (int i = 0; i < s1.size(); i++) {
        if (s1[0] != s1[i]) { return 0; }
    }
    for (int i = 0; i < s2.size(); i++) {
        if (s2[0] != s2[i]) { return 0; }
    }
    return 1;
}

bool ravenstvo(std::string s1, std::string s2, long long L) {
    if (s1.size() == L || s2.size() == L) { return 1; }
    return 0;
}

long long Exponentiation(long long n, short m) {
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
    return ans;
}


int main() {
    short t;
    std::cin >> t;
    for (int i = 0; i < t; i++) {
        short m;
        long long L, n;
        std::string s1, s2, s_suf1, s_pre1, s_suf2, s_pre2;
        std::cin >> L >> m >> s1 >> s2;
        n = L - s1.size() - s2.size();
            for (int i_suf = s1.size() + n, i_pre = 0; i_suf < s1.size(); i_suf++, i_pre++) {
            s_pre1 += s1[i_pre];
            s_suf1 += s1[i_suf];
        }
        for (int i_suf = s2.size() + n, i_pre = 0; i_suf < s2.size(); i_suf++, i_pre++) {
            s_pre2 += s2[i_pre];
            s_suf2 += s2[i_suf];
        }
        long long for_modyl = s1.size() - s2.size();
        if (s1.size() > L || s2.size() > L) { std::cout << 0 << '\n'; }
        else if (s1 == s2 && s1.size() + s2.size() == L) {
            std::cout << 1 << '\n';
        }
        else if (check_first_last(s1, s2, L) && n > 0) {
            std::cout << (2 * Exponentiation(n, m) - Exponentiation(n - abs(for_modyl), m)) % m << '\n';
        }
        else if (n > 0 && s1 == s2) {
            std::cout << Exponentiation(n, m) % m << '\n';
        }
        else if (n > 0) {
            std::cout << (2 * Exponentiation(n, m)) % m << '\n';
        }
        else if (ravenstvo(s1,s2,L) && s_pre1 == s2 || s_suf1 == s2) {
            std::cout << 1 % m << '\n';
        }
        else if (ravenstvo(s1,s2,L) && s_pre2 == s1 || s_suf2 == s1) {
            std::cout << 1 % m << '\n';
        }
        else if (s1.size() + s2.size() >= L && s1.size() + s2.size() - s_pre1.size() == L && s_pre1 == s_suf2 && s_pre2 == s_suf1 && !one_symbol(s1,s2,L)) {
            std::cout << 2 % m << '\n';
        }
        else if (s1.size() + s2.size() >= L && s1.size() + s2.size() - s_pre1.size() == L && s_pre1 == s_suf2 || s_pre2 == s_suf1) {
            std::cout << 1 % m << '\n';
        }
        else { std::cout << 0 << '\n'; }
    }
    return 0;
}