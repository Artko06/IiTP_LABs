#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
    int n;
	long double b1, q, S;
	cout << "Значение b1: ";
	cin >> b1;
	cout << "Ваш номер в журнале: ";
	cin >> n;
	q = 1.0 / (n + 1);
	S = b1 / (1 - q);
	cout << "Сумма бесконечно убывающей геометрической прогрессии: " << S;
	return 0;
}