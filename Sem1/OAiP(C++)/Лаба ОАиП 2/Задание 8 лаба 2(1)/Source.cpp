#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double a, b, c, D, t1, t2, x1, x2, x3, x4, x5, x6, t3, x7, x8;
	cout << "������� �,b,c �������������: ";
	cin >> a >> b >> c;
	//t = x * x;
	// at^2+bt+c=0
	D = b * b - 4 * a * c;
	t1 = (-b + sqrt(D)) / (2 * a);
	t2 = (-b - sqrt(D)) / (2 * a);
	x1 = -sqrt(t1);
	x2 = sqrt(t1);
	x3 = -sqrt(t2);
	x4 = sqrt(t2);
	x5 = sqrt(-c / b);
	x6 = -sqrt(-c / b);
	t3 = sqrt(-c / a);
	x7 = sqrt(t3);
	x8 = -sqrt(t3);
	if (a == 0 && b != 0) {
		if (-c / b > 0) {
			cout << "����� ���������: " << endl;
			cout << x5 << endl << x6 << endl;
		}
		else if (-c / b == 0) {
			cout << "������ ���������: " << endl;
			cout << 0 << endl;
		}
		else if (-c / b < 0) {
			cout << "��� ������" << endl;
		}
	}
	else if (a != 0 && b == 0) {
			if (-c / a > 0) {
				cout << "����� ���������: " << endl;
				cout << x7 << endl << x8 << endl;
			}
			else if (-c / a == 0) {
				cout << "������ ���������: " << endl;
				cout << 0 << endl;
			}
			else if (-c / a < 0) {
				cout << "��� ������" << endl;
			}
	}
	else if (a == 0 && b == 0 && c != 0) {
		cout << "ERROR";
	}
	else if (a == 0 && b == 0 && c == 0) {
		cout << "������";
	}
	else if (D > 0) {
		cout << "����� ���������: " << endl;
		if (t1 > 0) {
			cout << x1 << endl << x2 << endl;
		}
		else if (t1 == 0) {
			cout << 0 << endl;
		}
		if (t2 > 0) {
			cout << x3 << endl << x4 << endl;
		}
		else if (t2 == 0 && t1 != 0) {
			cout << 0 << endl;
		}
		if (t1 < 0 && t2 < 0) {
			cout << "��� ������" << endl;
		}
	}
	else if (D == 0) {
		cout << "����� ���������: " << endl;
		if (t1 > 0) {
			cout << x1 << endl << x2 << endl;
		}
		else if (t1 == 0) {
			cout << 0 << endl;
		}
		if (t1 < 0) {
			cout << "��� ������" << endl;
		}
	}
	else if (D < 0) {
		cout << "��� ������" << endl;
	}
	return 0;
}