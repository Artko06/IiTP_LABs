#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char a = '+';
	while (a == '+') {
		unsigned long long x, n, sum = 0;
		short ost, N;
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
		for (long long i = 1; i <= n; i++) {
			x = i;
			N = log10(x) + 1;
			while (x > 0) {
				ost = x % 10;
				sum = sum + pow(ost, N);
				x = x / 10;
			}
			if (i == sum) {
				cout << sum << endl;
				sum = 0;
			}
			else { sum = 0; }
		}
		cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
		cin >> a;
	}
	return 0;
}