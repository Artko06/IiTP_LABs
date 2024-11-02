#include <iostream>
using namespace std;
int main() {
    setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
    long long N;
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
    long long i = sqrt(N);
    if (sqrt(N) != i) { i++; }
    long long p = i * i - (i - 1);
    cout << "Расположение числа:\n";
    if (N == p) {
        cout << i - 1 << ' ' << i - 1;
    }
    else if (p > N) {
        cout << ((i - 1) - (p - N)) << ' ' << i - 1;
    }
    else {
        cout << i - 1 << ' ' << ((i - 1) - (N - p));
    }
	cout << "\nВведите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
	cin >> c;
	}
    return 0;
}