#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	long long num, s;
	short p, t, ch = 0, z = 0, m = 0;
	bool a1 = 1, a2 = 1, a3 = 1, a4 = 1, a5 = 1, a6 = 1, a7 = 1, a8 = 1, a9 = 1, a0 = 1;
	cout << "Дано натуральное число. Требуется подсчитать количество всех различных цифр в числе и определить, какая цифра стоит в разряде натурального числа с индексом i\n";
	cout << "Введите натуральное число: ";
	while (true) {
		if (!(cin >> num)) { // Если завершилось сбоем
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
		if (num < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	cout << "Введите индекс: ";
	while (true) {
		if (!(cin >> p)) { // Если завершилось сбоем
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
		if (p < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	s = num;
	while (s != 0) {
		s = s / 10;
		ch++;
	}
	int* a = new int[ch];
	short i = 0;
	while (num != 0) {
		t = num % 10;
		a[i] = t;
		i = i + 1;
		num = num / 10;
	}
	for (int i = 0; i < ch; i++) {
		if (a[i] == 0 && a0) { a0 = 0; m++; }
		else if (a[i] == 1 && a1) { a1 = 0; m++; }
		else if (a[i] == 2 && a2) { a2 = 0; m++; }
		else if (a[i] == 3 && a3) { a3 = 0; m++; }
		else if (a[i] == 4 && a4) { a4 = 0; m++; }
		else if (a[i] == 5 && a5) { a5 = 0; m++; }
		else if (a[i] == 6 && a6) { a6 = 0; m++; }
		else if (a[i] == 7 && a7) { a7 = 0; m++; }
		else if (a[i] == 8 && a8) { a8 = 0; m++; }
		else if (a[i] == 9 && a9) { a9 = 0; m++; }
	}
	cout << "Количество различных цифр:\n";
	cout << m << '\n';
	cout << "Номера всех разрядов в числе N:\n";
	for (int i = 0; i < ch; i++) {
		if (a[i] == a[ch - p]) {
			cout << i << ' ';
		}
	}
	delete[] a;
	a = nullptr;
	cout << "\nВведите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
	cin >> c;
	}
	return 0;
}