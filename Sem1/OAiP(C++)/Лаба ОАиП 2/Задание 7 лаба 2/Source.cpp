#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long long N;
	long double Y, a, b, c;
	cout << "Введите N: ";
	cin >> N;
	cout << "Значения a,b,c сooтветственно: ";
	cin >> a >> b >> c;
	switch (N) {
	case 2: Y = b * c - a * a; break;
	case 56: Y = b * c; break;
	case 7: Y = a * a + c; break;
	case 3: Y = a - b * c; break;
	default: Y = (a + b) * (a + b) * (a + b);
	}
	cout << "Итог = " << Y;
	return 0;
}