#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	int str = 0;
	int stlb = 0;
	short p = 0, Nmin = 0;
	cout << "Подсчёт кол-вa локальных минимумов заданой матрицы А:\n";
	cout << "Введите кол-во строк матрицы: ";
	while (true) {
		if (!(cin >> str)) { // Если завершилось сбоем
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
		if (str < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	cout << "Введите кол-во столбцов матрицы: ";
	while (true) {
		if (!(cin >> stlb)) { // Если завершилось сбоем
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
		if (stlb < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	int** a = new int* [str];
	for (int i = 0; i < str; i++) {
		a[i] = new int[stlb];
	}
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			cout << p + 1 << " Строка " << i + 1 << " Cтобец: ";
			while (true) {
				if (!(cin >> a[p][i])) { // Если завершилось сбоем
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
		}
	}
	p = 0;
	cout << "Матрица:\n";
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			cout << a[p][i] << '\t';
		}
		cout << '\n';
	}
	p = 0;

	if (str == 1) {
		for (; p < str; p++) {
			for (int i = 0; i < stlb; i++) {
				if (p == 0 && i == 0) {
					if (a[p][i] < a[p][i + 1]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == 0 && i == stlb - 1) {
					if (a[p][i] < a[p][i - 1]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else {
					if (a[p][i] < a[p][i - 1] && a[p][i] < a[p][i + 1]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
			}
		}
	}
	else if (stlb == 1) {
		for (; p < str; p++) {
			for (int i = 0; i < stlb; i++) {
				if (p == 0 && i == 0) {
					if (a[p][i] < a[p + 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == str - 1 && i == 0) {
					if (a[p][i] < a[p - 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p + 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
			}
		}
	}
	else {
		for (; p < str; p++) {
			for (int i = 0; i < stlb; i++) {
				if (p > 0 && p < str - 1 && i > 0 && i < stlb - 1) {
					if (a[p][i] < a[p][i + 1] && a[p][i] < a[p - 1][i] && a[p][i] < a[p + 1][i] && a[p][i] < a[p][i - 1]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == 0 && i == 0) {
					if (a[p][i] < a[p][i + 1] && a[p][i] < a[p + 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == str - 1 && i == stlb - 1) {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p][i - 1]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == 0 && i == stlb - 1) {
					if (a[p][i] < a[p][i - 1] && a[p][i] < a[p + 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == str - 1 && i == 0) {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p][i + 1]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (i == 0) {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p][i + 1] && a[p][i] < a[p + 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (i == stlb - 1) {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p][i - 1] && a[p][i] < a[p + 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == 0) {
					if (a[p][i] < a[p][i - 1] && a[p][i] < a[p][i + 1] && a[p][i] < a[p + 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == str - 1) {
					if (a[p][i] < a[p][i - 1] && a[p][i] < a[p][i + 1] && a[p][i] < a[p - 1][i]) {
						cout << "Элемент " << p + 1 << " строки " << i + 1 << " столбца - локальный минимум = " << a[p][i] << '\n';
						Nmin++;
					}
				}
			}
		}
	}
	cout << "Количество локальных минимумов матрицы = " << Nmin << '\n';

	for (int i = 0; i < str; i++) {
		delete[] a[i];
		a[i] = nullptr;
	}
	delete[] a;
	a = nullptr;
	cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
	cin >> c;
	}
	return 0;
}