#include <iostream>
#include <cmath>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double Q, x, p, n, K, D, B, C;
	cout << "��������� �������� ��� ��������: ";
	cin >> x;
	cout << "�������� �: ";
	cin >> p;
	cout << "�������� n: ";
	cin >> n;
	cout << "�������� K: ";
	cin >> K;
	cout << "�������� D: ";
	cin >> D;
	C = p - n;
	B = cos(x);
	Q = ((B * B) / (K * D)) + (C * C * C * B);
	cout << "�����: " << Q;
	return 0;
}