#include <iostream>
#include <cmath>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	// Найти время падения камня с высоты h
	float g = 9.81;
	long double t, h;
	cout << "Введите t: ";
	cin >> h;
	t = sqrt(2 * h / g);
	cout << "Время падения камня: " <<  t;
		return 0;
}