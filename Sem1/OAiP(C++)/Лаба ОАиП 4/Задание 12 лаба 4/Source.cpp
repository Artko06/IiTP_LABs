#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	long long N, w = 2, s = 3, z = 0;
	bool x = 1;
	cout << "Необходимо вывести строку и столбец в котором будет находится число, введенное с клавиатуры\n";
	cout << "Введите N: ";
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
		break;
	}
	long long **a = new long long *[sqrt(N)];
	for (int i = 0; i < sqrt(N); i++) {
		a[i] = new long long[sqrt(N)];
	}
	for (int p = 0; p < sqrt(N); p++) {
		for (int i = 0; i < sqrt(N); i++) {
			a[p][i] = 0;
		}
	}
	a[0][0] = 1;
	a[0][1] = 2;
	for (int p = 0; p < sqrt(N); p++) {
		for (; w < sqrt(N), s <= N; w++) {
			if (!x) {
				a[0][w - 1] = s;
				s++;
			}
			if (a[z + 1][0] == 0) {
				p++;
				for (int q = w - 1; q >= 0; q--) {
					a[p][q] = s;
					if (q != 0) {
						if (a[p][q - 1] != 0) {
							p++; 
							q++;
						}
					}
					s++;
				}
				z++;
				p = 0;
				x = 0;
				continue;
			}
			s++;
		}
	}
	cout << "Расположение числа:\n";
	for (int p = 0; p < sqrt(N); p++) {
		for (int i = 0; i < sqrt(N); i++) {
			if (a[p][i] == N) {
				cout << p << ' ' << i;
			}
		}
	}
	
	cout << '\n' << "Таблица:\n";
	for (int p = 0; p < sqrt(N); p++) {
		for (int i = 0; i < sqrt(N); i++) {
			cout << a[p][i] << '\t';
		}
		cout << '\n';
	}
	cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
	cin >> c;
	}
	return 0;
}