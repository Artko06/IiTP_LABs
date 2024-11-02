#include <iostream>
using namespace std;
int main() {
    setlocale(LC_ALL, "RU");
    char c = '+';
    while (c == '+') {
        long double x, sinys, cosinys, ln, sum1 = 0, sum2 = 0, sum3 = 0, p, l = 1, m = 0, s;
        long double N = 100000;
        short n = 9;
        long long factor = 1;
        cout << "Введите х: ";
        while (true) {
            if (!(cin >> x)) { // Если завершилось сбоем
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
            break;
        }
        if (x == 0) {
            cout << "ERROR ln|x|" << endl;
        }
        else if (abs(x) > exp(1)) {
            l = x;
            if (x < 0) {
                l = -l;
            }
            while (l > exp(1)) {
                l = l / exp(1);
                m++;
            }
            for (int i = 1; i <= N; i++) {
                ln = 1 / (i * powl(l / (l - 1), i));
                sum1 = sum1 + ln;
            }
            cout << "Значение ln|x| = " << sum1 + m << endl;
        }
        else if (abs(x) > 0 && abs(x) < 0.5) {
            l = x;
            if (x < 0) {
                l = -l;
            }
            for (int i = 1; i <= N; i++) {
                ln = powl(-1, i + 1) * powl((l - 1), i) / i;
                sum1 = sum1 + ln;
            }
            cout << "Значение ln|x| = " << sum1 << endl;
        }
        else {
            for (int i = 1; i <= N; i++) {
                ln = 1 / (i * powl(x / (x - 1), i));
                sum1 = sum1 + ln;
            }
            cout << "Значение ln|x| = " << sum1 + m << endl;
        }
        if (abs(x) > 1e5 * 2 * acos(-1)) {
            x = fmod(x, 2 * acos(-1)); 
        }
        if (x > 2 * acos(-1)) {
            for (double i = 2 * acos(-1); x >= 2 * acos(-1); x = x - i) {
            }
        }
        else if (x < -2 * acos(-1)) {
            for (double i = 2 * acos(-1); x <= -2 * acos(-1); x = x + i) {
            }
        }
        for (int i = 1, j = 1, p = 2; i <= n, j <= 2 * n, p <= 2 * n; i++, j = j + 2, p = p + 2) {
            factor = factor * j * p;
            cosinys = (powl((-1), i) * powl(x, 2 * i)) / factor;
            sum2 = sum2 + cosinys;
        }
        cout << "Значение cosx = " << 1 + sum2 << endl;
        factor = 1;
        for (int i = 1, j = 2, p = 3; i <= n, j <= 2 * n + 1, p <= 2 * n + 1; i++, j = j + 2, p = p + 2) {
            factor = factor * j * p;
            sinys = (powl((-1), i) * powl(x, 2 * i + 1)) / factor;
            sum3 = sum3 + sinys;
        }
        cout << "Значение sinx = " << x + sum3 << endl;
        if (x != 0) {
            if (x + sum3 <= 1 + sum2 && x + sum3 <= sum1 + m) {
                cout << "MIN: sinx = " << x + sum3 << endl;
            }
            else if (1 + sum2 <= x + sum3 && 1 + sum2 <= sum1 + m) {
                cout << "MIN: cosx = " << 1 + sum2 << endl;
            }
            else if (sum1 + m <= x + sum3 && sum1 + m <= 1 + sum2) {
                cout << "MIN: ln|x| = " << sum1 << endl;
            }
        }
        if (x == 0) {
            cout << "MIN: sinx = " << x + sum3 << endl;
        }
        cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
        cin >> c;
    }
    return 0;
}