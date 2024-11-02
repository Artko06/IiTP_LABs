#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char a = '+';
	while (a == '+') {
		float step, oblast, y, x = -1, r, v;
		cout << "Число на которое стоит разделить область поиска корня: ";
		while (true) {
			if (!(cin >> oblast)) { // Если завершилось сбоем
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
		if (oblast >= 1) {
			step = 2 / (oblast - 1);
			y = sin(x) + tan(x) - 1.0 / (1 + x * x);
			r = abs(y);
			while (x <= 1) {
				y = sin(x) + tan(x) - 1.0 / (1 + x * x);
				if (abs(y) <= r) {
					r = abs(y);
					v = x;
				}
				x = x + step;
			}
			cout << "Корень уравнения: " << v << endl;
		}
		else { cout << "Некоректно введены исходные данные" << endl; }
		cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
		cin >> a;
	}
	return 0;
}