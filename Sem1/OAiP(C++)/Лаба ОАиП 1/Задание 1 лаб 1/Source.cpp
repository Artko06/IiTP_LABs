#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	// Вычислите 23х^3+69x^2+32x+8 и -23х^3+69x^2-32x+8
	long double x;
	cout << "Введите значение x: ";
	cin >> x;
	long double y = x * x;
	long double z = x * (23 * y + 32);
	long double v = 69 * y + 8;
	cout << "Решение 1-го примера: " << z + v << endl;
	cout << "Решение 2-го примера: " << v - z << endl;
	return 0;
}