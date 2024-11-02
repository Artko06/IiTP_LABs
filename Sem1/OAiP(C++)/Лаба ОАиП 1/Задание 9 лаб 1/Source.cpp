#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	int x, y;
	cout << "Значение х изначально: " << endl;
	cin >> x;
	cout << "Значение y изначально: " << endl;
	cin >> y;
	cout << "Значение x и y соответсвенно до 1-го шага: " << x << '\t' << y << endl;
	if (x-y < INT_MIN || x-y > INT_MAX) {
		cout << "Возникло переполнение ячеек";
	}
	x = x - y;
	cout << "Значение x и y соответсвенно после 1-го шага: " << x << '\t' << y << endl;
	if (y + x < INT_MIN || y + x > INT_MAX) {
		cout << "Возникло переполнение ячеек";
	}
	y = y + x; // y стал начальным х
	if (x - y < INT_MIN || x - y > INT_MAX) {
		cout << "Возникло переполнение ячеек";
	}
	cout << "Значение x и y соответсвенно после 2-го шага: " << x << '\t' << y << endl;
	x = x - y;
	cout << "Значение x и y соответсвенно после 3-го шага: " << x << '\t' << y << endl;
	x = -x;
	cout << "Итог х: " << x << endl;
	cout << "Итог y: " << y << endl;
	return 0;
}