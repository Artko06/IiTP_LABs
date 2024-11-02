#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double x1, y1, x2, y2, r, R, d;
	cout << "Задайте координаты центра 1-ого круга x1 и y1 соотвественно: ";
	cin >> x1 >> y1;
	cout << "Радиус первого круга: ";
	cin >> r;
	cout << "Задайте координаты центра 2-ого круга x2 и y2 соотвественно: ";
	cin >> x2 >> y2;
	cout << "Радиус второго круга: ";
	cin >> R;
	d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	if (d > R + r) {
		cout << "Ни одно условие не выполнено" << endl; // круги не пересекаются
	}
	else if (r > R) {
		if (d + R < r) {
			cout << "Да, но справедливо обратное для фигур" << endl;; // круг 1 вложен в круг 2
		}
		else if (d + R >= r) {
			cout << "Фигуры пересекаются" << endl;;
		}
	}
	else if (R > r) {
		if (d + r < R) {
			cout << "Да" << endl;; // круг 2 вложен в круг 1
		}
		else if (d + r >= R) {
			cout << "Фигуры пересекаются" << endl;
		}
	}
	else if (R == r) {
		cout << "Фигуры пересекаются" << endl;
	}
	return 0;
}