#include <iostream>

bool check(const int* a) {
    int k = a[0];
    int* b = new int[k];
    int pos = 0;
    for (int i = 201 - k; i < 201; i++) b[pos++] = a[i]; 

    int ost;
    while (k--) { 
        int end = 0;
        for (int i = 0; i < pos; i++) {
            int x = b[i] + 10 * end;
            end = x % 2;
            b[i] = x / 2;
        }
        ost = b[pos - 1] & 1;
    }
    delete[]b;
    return ost == 0;
}


int main() {
    int x = 0;
    std::cin >> x;
    int** a = new int* [20001];
    for (int i = 0; i < 20001; i++) {
        a[i] = new int[201] {};
    }

    int k = 200, pos = 2, start = 1, end = 1;

    a[1][k] = 1;
    a[1][0] = 1;

    bool was0 = false;
    do { 
        for (int i = start; i < end + 1; i++) {
            int j;
            for (j = k; j > k - a[i][0]; j--)
                a[pos][j] = a[i][j];

            if (!was0) a[pos][k - a[i][0] + 1] = 0;

            a[pos][k - a[i][0]] = 1;
            a[pos][0] = a[i][0] + 1;

            if (check(a[i])) pos++;
        }
        if (was0) was0 = false;
        else was0 = true;

        if (!was0) {
            start = end + 1;
            end = pos - 1;
        }

    } while (pos <= x);

    for (int j = k - a[x][0] + 1; j <= k; j++) { 
        std::cout << a[x][j];
    }
    std::cout << "\n";

    return 0;
}