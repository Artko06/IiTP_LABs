#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	const short n = 4, m = 3;
	long long a[n][m], sum = 0;
	short p;
	cout << "Найти сумму элементов рассположенных в чётных(по номеру) строках\n";
	for (int p = 0; p < n; p++) {
		for (int i = 0; i < m; i++) {
			cout << p + 1 << " cтрока " << i + 1 << " cтолбец\n";
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
		if ((p + 1) % 2 == 0) {
			for (int i = 0; i < m; i++) {
				sum = sum + a[p][i];
			}
		}
	} 
	cout << "Сумма элементов в чётных строках матрицы: " << sum << '\n';
	cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
	cin >> c;
	}
	return 0;
}