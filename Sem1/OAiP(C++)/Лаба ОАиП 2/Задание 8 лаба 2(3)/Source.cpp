#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double p, q, D, x, x1, x2, x3;
	cout << "Значения p и q соответвенно: ";
	cin >> p >> q;
	D = (p / 3) * (p / 3) * (p / 3) + (q / 2) * (q / 2);
	x = cbrtl((-q / 2) + sqrt(D)) + cbrtl((-q / 2) - sqrt(D));
	if (p == 0 && q != 0) {	cout << "x = " << cbrtl(-q); }
	else if (q == 0) {
		if (p >= 0) {
			cout << "x = " << 0;
		}
		else if (p < 0) {
			cout << "x1 = " << 0 << endl << "x2 = " << sqrt(-p) << endl << "x3 = " << -sqrt(-p);
		}
	}
	else if (D >= 0) {
		cout << "Уравнение имеет 1 действительный корень:" << endl << "x = " << x;
	}
	else if (D < 0) {
		x1 = 2 * sqrt(-p / 3) * cos(1 / 3.0 * acos((3 * q) / (2 * p) * sqrt(-3 / p)));
		x2 = 2 * sqrt(-p / 3) * cos(1 / 3.0 * acos((3 * q) / (2 * p) * sqrt(-3 / p)) - 2 * acos(-1) / 3);
		x3 = 2 * sqrt(-p / 3) * cos(1 / 3.0 * acos((3 * q) / (2 * p) * sqrt(-3 / p)) - 4 * acos(-1) / 3);
		cout << "D < 0(уравнение имеет 3 действительных корня)" << endl;
		cout << "x1 = " << x1 << endl << "x2 = " << x2 << endl << "x3 = " << x3;
	}
	return 0;
}