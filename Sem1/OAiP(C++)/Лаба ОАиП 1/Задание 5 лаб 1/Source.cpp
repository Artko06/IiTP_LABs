#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	int n;
	long double a1, a100, d, S;
	cout << "Первый член арифметической прогрессии а1: ";
	cin >> a1;
	cout << "Сотый член арифметической прогрессии а100: ";
	cin >> a100;
	cout << "Ваш номер в журнале n: ";
	cin >> n;
	d = (a100 - a1) / 99;
	cout << "Разность арифметической прогрессии: " << d << endl;
	S = ((2 * a1 + d*(n + 10 - 1)) / 2) * (n+10);
	cout << "Cумма первых n+10 членов арифметической прогрессии: " << S;
	return 0;
}