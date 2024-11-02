#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double x, y, z;
	cout << "Первая сторона равна: ";
	cin >> x;
	cout << "Вторая сторона равна: ";
	cin >> y;
	cout << "Третья сторона равна: ";
	cin >> z;
	if (x <= 0 || y <= 0 || z <= 0) {
		cout << "ERROR";
	}
	else if (x + y <= z || x + z <= y || y + z <= x) {
		cout << "Такой треугольник не существует";
	}
	else { cout << "Такой треугольник существует"; 
	}
	return 0;
}
