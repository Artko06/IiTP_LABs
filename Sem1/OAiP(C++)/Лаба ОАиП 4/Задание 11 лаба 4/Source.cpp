#include <iostream>
using namespace std;
int main() {
    setlocale(LC_ALL, "RU");
    char c = '+';
    while (c == '+') {
    int n, m;
    char p = 48;
    cout << "На вход подаётся поле для игры в Cапёр размером n*m символов, где символ '.' означает пустое место, а символ '*' - бомбу. Требуется дополнить это поле числами, как в оригинальной игре\n";
    cout << "Длина и ширина поля соотвественно: ";
    while (true) {
        if (!(cin >> m)) { // Если завершилось сбоем
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
        if (m < 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод \n";
            continue;
        }
        break;
    }
    while (true) {
        if (!(cin >> n)) { // Если завершилось сбоем
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
        if (n < 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод \n";
            continue;
        }
        break;
    }
    char** a = NULL;
    a = (char**)malloc((n + 2) * sizeof(char*));
    for (int i = 0; i < n + 2; i++) {
        a[i] = (char*)malloc((m + 2) * sizeof(char));
    }
    for (int q = 0; q < n + 2; q++) {
        for (int i = 0; i < m + 2;) {
            if (q == 0 || q == n + 1 || i == 0 || i == m + 1) {
                a[q][i] = '.';
                i++;
                continue;
            }
            cout << i << " клетка поля " << q << " cтороки: ";
            cin >> a[q][i];
            if (a[q][i] == '*' || a[q][i] == '.') { i++; }
        }
    }
    for (int q = 1; q < n + 1; q++) {
        for (int i = 1; i < m + 1; i++) {
            if (a[q][i] == '.') {
                for (int w = q - 1; w < q + 2; w++) {
                    for (int e = i - 1; e < i + 2; e++) {
                        if (a[w][e] == '*') { p++; }
                    }
                }
                if (p != 48) { a[q][i] = p; }
                p = 48;
            }
        }
    }
    cout << "Поле для игры в сапёра:\n";
    for (int q = 1; q < n + 1; q++) {
        for (int i = 1; i < m + 1; i++) {
            cout << a[q][i] << "\t ";
        }
        cout << "\n\n";
    }
    for (int i = 0; i < n + 2; i++) {
        free(a[i]);
        a[i] = NULL;
    }
    free(a);
    a = NULL;
    cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    cin >> c;
}
    return 0;
}