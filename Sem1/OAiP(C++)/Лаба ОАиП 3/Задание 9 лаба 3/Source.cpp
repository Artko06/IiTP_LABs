#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
		long long F, a, n, sum = 0, p = 0;
		cout << "Введите a: ";
		while (true) {
			if (!(cin >> a)) { // Если завершилось сбоем
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
		cout << "Введите n: ";
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
		if (n <= 0) {
			cout << "ERROR" << endl;
		}
		for (long long i = 0; i <= n - 1; i++) {
			F = powl(2, i) * powl((-1), i) * a;
			sum = sum + F;
			if (F >= LLONG_MAX - powl(2, i - 1) * powl((-1), i - 1) * a || sum >= LLONG_MAX - powl(2, i - 1) * powl((-1), i - 1) * a) {
				p = 1;
				cout << "Для данных чисел происходит переполнение" << endl;
				break;
			}
		}
		if (p != 1 && n > 0) {
			cout << "Итог =  " << sum << endl;
		}
		cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
		cin >> c;
	}
	return 0;
}