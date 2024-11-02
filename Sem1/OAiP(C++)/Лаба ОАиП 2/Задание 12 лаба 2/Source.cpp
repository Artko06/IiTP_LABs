#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double a, b, c, x, y;
	cout << "Размеры кирпича a,b,c соответсвенно: ";
	cin >> a >> b >> c;
	cout << "Размеры отверстия: ";
	cin >> x >> y;
	if (a <= 0 || b <= 0 || c <= 0 || x <= 0 || y <= 0) {
		cout << "ERROR" << endl;
	}
	else if (a * b <= x * y || a * c <= x * y || b * c <= x * y) {
		cout << "Кирпич пройдёт в отверстие" << endl;
	}
	else { cout << "Кирпич НЕ пройдёт в отверстие" << endl; }
	return 0;
}