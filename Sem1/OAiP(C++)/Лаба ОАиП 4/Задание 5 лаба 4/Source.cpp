#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	long long str, stlb, p = 0;
	cout << "Для заданной матрицы А размером n*m построить матрицу В такого же размера, элементы которой обладают следующим свойством: элемент B[i][j] равен максимальному из элементов матрицы А, расположенных левее и выше позиции (i, j), включая позицию(i, j).При этом считается, что позиция (1, 1) - верхняя левая позиция матрицы\n";
	cout << "Введите кол-во строк в матрице: ";
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
	cout << "Введите кол-во столбцов в матрице: ";
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
	int** b = new int* [str];
	for (int i = 0; i < str; i++) {
		a[i] = new int[stlb];
	}
	cout << "Матрица А:\n";
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
	for (int i = 0; i < str; i++) {
		b[i] = new int[stlb];
	}
	cout << "Матрица А:\n";
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			cout << a[p][i] << '\t';
		}
		cout << '\n';
	}
	p = 0;

	for (int i = 0; i < str; i++) {
		int max = a[i][0];
		for (int j = 0; j < stlb; j++) {
			if (max < a[i][j]) max = a[i][j];
			b[i][j] = max;
		}
	}
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			if (p == str - 1 && i == stlb - 1) break;
			else if (p == str - 1) continue;
			if (b[p][i] > b[p + 1][i]) {
				for (int k = p + 1; k < str; k++) {
					if(b[k][i] < b[p][i]) b[k][i] = b[p][i];
				}
			}
		}
	}
	p = 0;
	cout << "Матрица B:\n";
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			cout << b[p][i] << '\t';
		}
		cout << '\n';
	}

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