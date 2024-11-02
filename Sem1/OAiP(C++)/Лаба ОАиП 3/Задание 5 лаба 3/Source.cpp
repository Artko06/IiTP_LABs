#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char a = '+';
	while (a == '+') {
		long double F, n, sum = 0, ln, N, p = 1, x;
		cout << "Значение n: ";
		while (true) {
			if (!(cin >> n)) { // Если завершилось сбоем
				cin.clear(); //Снимает флаг ошибки при cin (чтобы будущие операции ввода-вывода работали корректно)
				while (cin.get() != '\n'); //читает из потока один символ
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
		cout << "Кол-во рассчётов: ";
		while (true) {
			if (!(cin >> N)) { // Если завершилось сбоем
				cin.clear(); //Снимает флаг ошибки при cin (чтобы будущие операции ввода-вывода работали корректно)
				while (cin.get() != '\n'); //читает из потока один символ
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
		if (N >= 1 && n >= 1) {
			while (p <= N) {
				cout << "Введите х в промежутке от 0.1 до 1: ";
				cin >> x;
				if (x >= 0.1 && x <= 1) {
					int i = 1;
					while (i <= n) {
						F = powl((-1), i + 1) * powl(-(x * x + 2 * x + 1) / (x * x + 2 * x + 2), i) / i;
						sum = sum + F;
						i++;
					}
					cout << "Итог ряда: " << sum << endl;
					sum = 0;
					ln = log(1.0 / (2 + 2 * x + x * x));
					cout << "Итог ln: " << ln << endl;
					p++;
				}
				else {
					cout << "Исходные данные некоректны" << endl;
					break;
				}
			}
		}
		else { cout << "Исходные данные некоректны" << endl; }
		cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
		cin >> a;
	}
	return 0;
}