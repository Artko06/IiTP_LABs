#include <iostream>
#include <cmath>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	// ����� ����� ������� ����� � ������ h
	float g = 9.81;
	long double t, h;
	cout << "������� t: ";
	cin >> h;
	t = sqrt(2 * h / g);
	cout << "����� ������� �����: " <<  t;
		return 0;
}