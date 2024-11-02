#include <iostream>
using namespace std;
int main() {
    setlocale(LC_ALL, "RU");
    char d = '+';
    while (d == '+') {
    int N;
    bool c = 1;
    long long z = 1;
    cout << "Построить магический квадрат. Порядок квадрата задаётся пользователем\n";
    cout << "Введите порядок квадрата: ";
    while (true) {
        if (!(cin >> N)) { // Если завершилось сбоем
            cin.clear(); //Снимает флаг ошибки при cin (чтобы будущие операции ввода-вывода работали корректно)
            while (cin.get() != '\n'); // читает из потока один символ
            cout << "Неверный ввод \n";
            continue;
        }
        if (cin.peek() != '\n') { //заглядывает во входной поток и сообщает нам, каким будет следующий символ
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Удалить символы из буфера
            cout << "Неверный ввод \n";
            continue;
        }
        if (N < 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод \n";
            continue;
        }
        if (N == 2 || N == 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Невозможно построить магический квадрат. Повторите ввод \n";
            continue;
        }
        break;
    }
    long long** a = NULL;
    a = (long long**)malloc((N) * sizeof(long long*));
    for (long long i = 0; i < N; i++) {
        a[i] = (long long*)malloc((N) * sizeof(long long));
    }
    if (N % 2 != 0) {
        for (long long i = N / 2, q = 0; z <= N * N; i++, z++, q--) {
            if (a[0][N - 1] > 0 && c) {
                q = 1;
                i = N - 1;
                c = 0;
            }
            else if (a[q][i] > 0) {
                q += 2;
                i--;
            }
            a[q][i] = z;
            if (q == 0) { q = N; }
            if (i == N - 1) { i = -1; }
        }
        for (long long p = 0; p < N; p++) {
            for (long long i = 0; i < N; i++) {
                cout << a[p][i] << " ";
            }
            cout << '\n';
        }
    }
    else if (N % 4 == 0) {
        for (int q = 0; q < N; q++) {
            for (int i = 0; i < N; i++) {
                a[q][i] = z;
                z++;
            }
        }
        for (int q = 0; q < N / 4; q++) {
            for (int i = 0; i < N / 4; i++) {
                a[q][i] = 0;
            }
        }
        for (int q = 0; q < N / 4; q++) {
            for (int i = N - N / 4; i < N; i++) {
                a[q][i] = 0;
            }
        }
        for (int q = N - N / 4; q < N; q++) {
            for (int i = 0; i < N / 4; i++) {
                a[q][i] = 0;
            }
        }
        for (int q = N - N / 4; q < N; q++) {
            for (int i = N - N / 4; i < N; i++) {
                a[q][i] = 0;
            }
        }
        for (int q = N / 4; q < N - N / 4; q++) {
            for (int i = N / 4; i < N - N / 4; i++) {
                a[q][i] = 0;
            }
        }
        z = N * N;
        for (int q = 0; q < N; q++) {
            for (int i = 0; i < N; i++) {
                if (a[q][i] != 0) {
                    z--;
                    continue;
                }
                a[q][i] = z;
                z--;
            }
        }
        for (long long p = 0; p < N; p++) {
            for (long long i = 0; i < N; i++) {
                cout << a[p][i] << " ";
            }
            cout << '\n';
        }
    }
    else { // N делится на 2, но не делится на 4 
        for (long long i = N / 4, q = 0; z <= N * N / 4; i++, z++, q--) {
            if (a[0][N / 2 - 1] > 0 && c) {
                q = 1;
                i = N / 2 - 1;
                c = 0;
            }
            else if (a[q][i] > 0) {
                q += 2;
                i--;
            }
            a[q][i] = z;
            if (q == 0) { q = N / 2; }
            if (i == N / 2 - 1) { i = -1; }
        }
        for (int q = N / 2, w = 0; q < N, w < N / 2; q++, w++) {
            for (int i = N / 2, e = 0; i < N, e < N / 2; i++, e++) {
                a[q][i] = a[w][e];
                a[q][i] += N * N / 4;
            }
        }

        for (int q = N / 2, w = 0; q < N, w < N / 2; q++, w++) {
            for (int i = N / 2, e = 0; i < N, e < N / 2; i++, e++) {
                a[q][i] = a[w][e];
                a[q][i] += N * N / 4;
            }
        }

        for (int q = N / 2, w = 0; q < N, w < N / 2; q++, w++) {
            for (int i = 0, e = 0; i < N / 2, e < N / 2; i++, e++) {
                a[q][i] = a[w][e];
                a[q][i] += 3 * N * N / 4;
            }
        }

        for (int q = 0, w = 0; q < N / 2, w < N / 2; q++, w++) {
            for (int i = N / 2, e = 0; i < N, e < N / 2; i++, e++) {
                a[q][i] = a[w][e];
                a[q][i] += 2 * N * N / 4;
            }
        }

        int t = N / 4;
        if (N > 9) {
            for (int q = 0, w = N / 2; q < N / 2, w < N; q++, w++) {
                for (int i = N - ((N - 6) / 4), r = 0; i < N, r < t; i++, r++) {
                    swap(a[q][i], a[w][i]);
                    swap(a[q][r], a[w][r]);
                }
            }
            swap(a[t][0], a[N - 1 - t][0]);
            swap(a[t][t], a[N - 1 - t][t]);
        }
        else if (N == 6) {
            swap(a[0][0], a[3][0]);
            swap(a[1][1], a[4][1]);
            swap(a[2][0], a[5][0]);
        }

        for (long long p = 0; p < N; p++) {
            for (long long i = 0; i < N; i++) {
                cout << a[p][i] << " ";
            }
            cout << '\n';
        }
    }
    for (long long i = 0; i < N; i++) {
        free(a[i]);
        a[i] = NULL;
    }
    free(a);
    a = NULL;
    cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    cin >> d;
    }
    return 0;
}