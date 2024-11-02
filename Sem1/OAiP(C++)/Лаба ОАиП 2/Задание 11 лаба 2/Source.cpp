#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double S, P, Q, R, Qrad, r;
	const double PI = acos(-1);
	cout << "Радиус шара: ";
	cin >> R;
	cout << "Длина стороны ромбовидного отверстия: ";
	cin >> P;
	cout << "Острый угол ромба в градусах: ";
	cin >> Q;
	Qrad = (PI * Q) / 180;
	S = P * P * sin(Qrad);
	r = S / (2 * P);
	if (R <= 0 || P <= 0 || Q <= 0 || Q >= 90) {
		cout << "Ввод данных произведён некоректно";
	}
	else if (R <= r) {
		cout << "Шар пройдёт через ромбовидное отверстие";
	}
	else { cout << "Шар НЕ пройдёт через ромбовидное отверстие"; }
}