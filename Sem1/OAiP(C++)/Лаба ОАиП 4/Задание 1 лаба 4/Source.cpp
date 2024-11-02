#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	const short k = 5;
	int a[k], sum = 0, sum2 = 0;
	double sr;
	cout << "Cуть задания:\n";
	cout << "Ввести одномерный статический массив из k чисел. Определить кол-во элементов больше среднего\n";
	for (int i = 0; i < k; i++) {
		cout << i << " Элемент массива: ";
		while (true) {
			if (!(cin >> a[i])) { // Если завершилось сбоем
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
		sum = sum + a[i];
	}
	sr = sum / k;
	cout << "Элементы больше среднего: \n";
	for (int i = 0; i < k; i++) {
		if (a[i] > sr) {
			cout << a[i] << " ";
			sum2++;
		}
	}
	cout << '\n' << "Количество: " << sum2 << '\n';
	cout << "Введите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
	cin >> c;
	}
	return 0;
}