#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double S, P, Q, R, r;
	const double PI = acos(-1);
	cout << "Радиус шара: ";
	cin >> R;
	cout << "Длина стороны ромбовидного отверстия: ";
	cin >> P;
	cout << "Острый угол ромба в радианах: ";
	cin >> Q;
	S = P * P * sin(Q);
	r = S / (2 * P);
	if (R <= 0 || P <= 0 || Q <= 0 || Q >= PI/2) {
		cout << "Ввод данных произведён некоректно";
	}
	else if (R <= r) {
		cout << "Шар пройдёт через ромбовидное отверстие";
	}
	else { cout << "Шар НЕ пройдёт через ромбовидное отверстие"; }
}