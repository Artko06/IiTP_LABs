#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double a, b, c, D1, D2, D3, D4, t1, t2, t3, x1, x2, x3, x4, x5, x6;
	cout << "Введите а,b,c соответсвенно: ";
	cin >> a >> b >> c;
	//t = x + 1/x;
	// at^2+bt+(c-2a)=0
	D1 = b * b - 4 * a * (c - 2 * a);
	t1 = (-b + sqrt(D1)) / (2 * a);
	t2 = (-b - sqrt(D1)) / (2 * a);
	t3 = (2 * a - c) / b;
	D2 = t1 * t1 - 4;
	D3 = t2 * t2 - 4;
	D4 = c * c - 4 * b * b;
	x1 = (t1 + sqrt(D2)) / 2;
	x2 = (t1 - sqrt(D2)) / 2;
	x3 = (t2 + sqrt(D3)) / 2;
	x4 = (t2 - sqrt(D3)) / 2;
	x5 = (-c + sqrt(D4)) / (2 * b);
	x6 = (-c - sqrt(D4)) / (2 * b);
	if (a == 0 && b != 0) {
		if (D4 > 0) {
			cout << "Корни уравнения: " << endl;
			cout << x5 << endl << x6 << endl << 0 << endl;
		}
		else if (D4 == 0) {
			cout << "Корни уравнения: " << endl;
			cout << x5 << endl << 0 << endl;
		}
		else if (D4 < 0) {
			cout << "Корень уравнения: " << endl;
			cout << 0 << endl;
		}
	}
	else if (a == 0 && b == 0 && c != 0) {
		cout << "x = " << 0;
	}
	else if (a == 0 && b == 0 && c == 0) {
		cout << "Истина";
	}
	else if (D1 > 0) {
		cout << "Корни уравнения: " << endl;
		if (D2 > 0) {
			cout << x1 << endl << x2 << endl;
		}
		else if (D2 == 0) {
			cout << x1 << endl;
		}
		if (D3 > 0) {
			cout << x3 << endl << x4 << endl;
		}
		else if (D3 == 0) {
			cout << x3 << endl;
		}
		if (D2 < 0 && D3 < 0) { cout << "Нет корней" << endl; }
	}
	else if (D1 == 0) {
		cout << "Корни уравнения: " << endl;
		if (D2 > 0) {
			cout << x1 << endl << x2 << endl;
		}
		else if (D2 == 0) {
			cout << x1 << endl;
		}
		else { cout << "Нет корней" << endl; }
	}
	else if (D1 < 0) { cout << "Нет корней" << endl; }
	return 0;
}