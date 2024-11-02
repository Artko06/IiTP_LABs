#include <iostream>
using namespace std;
int main() {
	char c = '+';
	while (c == '+') {
		setlocale(LC_ALL, "RU");
		long long b, sum;
		float a;
		cout << "До какого числа будет происходить суммирование: ";
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
		b = (int)a;
		if (b > 0) {
			if (b < 2) {
				cout << "Cумма чётных чисел равна: " << 0 << endl;
			}
			else if (b >= 2) {
				if (b % 2 != 0) {
					b--;
				}
				sum = (b * b + 2 * b) / 4;
				cout << "Cумма чётных чисел равна: " << sum << endl;
			}
		}
		else { cout << "Некоректно введены исходные данные" << endl; }
		cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
		cin >> c;
	}
	return 0;
}