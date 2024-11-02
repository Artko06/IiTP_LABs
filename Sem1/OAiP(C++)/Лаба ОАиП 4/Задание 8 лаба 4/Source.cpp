#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char d = '+';
	while (d == '+') {
	long long stra, strb, strc, m = 0, sum = 0;
	cout << "Необходимо найти произведение матриц A * B\n";
	cout << "Введите размер матрицы А: ";
	while (true) {
		if (!(cin >> stra)) { // Если завершилось сбоем
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
		if (stra < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	strc = stra;
	int** a = new int*[stra];
	for (int i = 0; i < stra; i++) {
		a[i] = new int[stra];
	}
	for (int p = 0; p < stra; p++) {
		for (int i = 0; i < stra; i++) {
			cout << p + 1 << " Строка " << i + 1 << " Столбец: ";
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
	cout << "Матрица A:\n";
	for (int p = 0; p < stra; p++) {
		for (int i = 0; i < stra; i++) {
			cout << a[p][i] << '\t';
		}
		cout << '\n';
	}
	cout << "Введите размер матрицы B: ";
	while (true) {
		if (!(cin >> strb)) { // Если завершилось сбоем
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
		if (strb < 1 || strb != stra) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	int** b = new int* [strb];
	for (int i = 0; i < strb; i++) {
		b[i] = new int[strb];
	}
	for (int p = 0; p < strb; p++) {
		for (int i = 0; i < strb; i++) {
			cout << p + 1 << " Строка " << i + 1 << " Столбец: ";
			while (true) {
				if (!(cin >> b[p][i])) { // Если завершилось сбоем
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
	cout << "Матрица B:\n";
	for (int p = 0; p < strb; p++) {
		for (int i = 0; i < strb; i++) {
			cout << b[p][i] << '\t';
		}
		cout << '\n';
	}
	int** c = new int* [strc];
	for (int i = 0; i < stra; i++) {
		c[i] = new int[strc];
	}
	for (int p = 0; p < strc; p++) {
		for (int i = 0; i < strc; i++) {
			for (; m < strc; m++) {
				c[p][i] = a[p][m] * b[m][i];
				sum = sum + c[p][i];
			}
			c[p][i] = sum;
			sum = 0;
			m = 0;
		}
	}
	cout << "Матрица C:\n";
	for (int p = 0; p < strc; p++) {
		for (int i = 0; i < strc; i++) {
				cout << c[p][i] << ' ';
			//cout << c[p][i] << '\t';
		}
		cout << '\n';
	}
	for (int i = 0; i < stra; i++) {
		delete[] a;
		a = nullptr;
	}
	delete a;
	a = nullptr;
	for (int i = 0; i < stra; i++) {
		delete[] b;
		b = nullptr;
	}
	delete b;
	b = nullptr;
	for (int i = 0; i < stra; i++) {
		delete[] c;
		c = nullptr;
	}
	delete c;
	c = nullptr;
	cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
	cin >> d;
	}
	return 0;
}