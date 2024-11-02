#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char a = '+';
	while (a == '+') {
		double H = acos(-1) / 40, y;
		float x;
		while (true) {
			cout << "Введите шаг: ";
			if (!(cin >> x)) { // Если завершилось сбоем
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
		for (long double i = 10; i <= 20; i = i + x) {
			y = sqrt(2) * sin(i * H - 10 * H);
			cout << "Значения ф-ции: \n" << y << endl;
		}
		cout << "Введите '+', если желаете повторить или любой другой символ для завершения программы: ";
		cin >> a;
	}
	return 0;
}