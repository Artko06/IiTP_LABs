#include <iostream>
#include <cmath>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double Q, x, p, n, K, D, B, C;
	cout << "Радианное значение для косинуса: ";
	cin >> x;
	cout << "Значение р: ";
	cin >> p;
	cout << "Значение n: ";
	cin >> n;
	cout << "Значение K: ";
	cin >> K;
	cout << "Значение D: ";
	cin >> D;
	C = p - n;
	B = cos(x);
	Q = ((B * B) / (K * D)) + (C * C * C * B);
	cout << "Ответ: " << Q;
	return 0;
}