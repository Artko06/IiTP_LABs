#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	// Найти S, зная a, v0, t
	long double a, v0, t, S;
	cout << "Значения ускорения а: ";
	cin >> a;
	cout << "Значения начальной скорости v0: ";
	cin >> v0;
	cout << "Значения времени t: ";
	cin >> t;
	S = v0 * t + a * t * t / 2;
	cout << "Расстояние пройденное телом S: " << S;
	return 0;
}
