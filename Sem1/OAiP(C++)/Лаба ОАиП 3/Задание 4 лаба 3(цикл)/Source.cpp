#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
		long long sum = 0;
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
		a = (int)a;
		for (long long i = 1; i <= a; i++, cout << 1) {
			if (i % 2 != 0) {
				continue;
			}
			else if (i % 2 == 0) {
				sum = sum + i;
			}
		}
		cout << "Сумма всех чётных равна: " << sum << endl;
		cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
		cin >> c;
	}
	return 0;
}