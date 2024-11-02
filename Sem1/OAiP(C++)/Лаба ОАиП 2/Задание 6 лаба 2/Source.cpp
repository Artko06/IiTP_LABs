#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double Z, a, b, c, d;
	cout << "Значения a,b,c,d соответсвенно: ";
	cin >> a >> b >> c >> d;
	if (c == 0 && c >= d && a < d) {
		cout << "ERROR";
	}
	else if (c == 0 && c < d && a >= d) {
		cout << "ERROR";
	}
	else if (c >= d && a < d) {
		Z = a + b / c;
		cout << "Z = " << Z;
	}
	else if (c < d && a >= d) {
		Z = a - b / c;
		cout << "Z = " << Z;
	}
	else { Z = 0;
	cout << "Z = " << Z;
	}
	return 0;
}