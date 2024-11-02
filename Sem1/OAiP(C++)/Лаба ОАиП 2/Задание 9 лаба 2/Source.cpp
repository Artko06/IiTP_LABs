#include <iostream>
#include <cmath>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double z, x, k, m, n, F, f1, y;
	int f;
	cout << "13 Вариант задачи" << endl;
	cout << "Значение z: ";
	cin >> z;
	if (z > 1) {
		x = z;
	}
	else if (z <= 1) {
		x = z * z + 1;
	}
	cout << "Используя цифры 1,2 или 3 соответсвенно выбери значение для ф-ции f(x)" << endl;
	cout << "При 1: f(х) = 2х" << endl;
	cout << "При 2: f(х) = x^3" << endl;
	cout << "При 3: f(х) = х/3" << endl;
	cin >> f;
	switch (f) {
	case 1: f1 = 2 * x; break;
	case 2: f1 = x * x * x; break;
	case 3: f1 = x / 3; break;
	}
	if (f != 1 && f != 2 && f != 3) {
		cout << "ERROR" << endl;
		cout << "Выбери функцию используя цифры 1,2 или 3" << endl;
	}
	else {
		cout << "Значение k: ";
		cin >> k;
		cout << "Значение m: ";
		cin >> m;
		cout << "Значение n: ";
		cin >> n;
		if (m * x <= 0) {
			cout << "Не определён";
		}
		else {
			F = int(f1 * n); // к синусу
			y = sin(F) + cos(k * x) + log(m * x);
			cout << y << endl;
		}
	}
	return 0;
}