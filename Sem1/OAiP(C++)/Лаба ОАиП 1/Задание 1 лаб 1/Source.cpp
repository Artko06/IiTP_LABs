#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	// ��������� 23�^3+69x^2+32x+8 � -23�^3+69x^2-32x+8
	long double x;
	cout << "������� �������� x: ";
	cin >> x;
	long double y = x * x;
	long double z = x * (23 * y + 32);
	long double v = 69 * y + 8;
	cout << "������� 1-�� �������: " << z + v << endl;
	cout << "������� 2-�� �������: " << v - z << endl;
	return 0;
}