#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long long X, Y;
	long double A, B, C, K;
	cout << "������� �������� ���������� � � � �������������:";
	cin >> X >> Y;
	if (X > Y) {
		Y = 0;
	}
	else if (Y > X) {
		X = 0;
	}
	else if (X = Y) {
		X = 0;
		Y = 0;
	}
	cout << "������� �������� ���������� A,B,C �������������:";
	cin >> A >> B >> C;
	cout << "������� K: ";
	cin >> K;
	if (A > B && A > C) {
		A = A - K;
	}
	else if (B > A && B > C) {
		B = B - K;
	}
	else if (C > A && C > B) {
		C = C - K;
	}
	else {
		cout << "����� A,B,C ��� ������������� ����������� �����" << endl;
	}
	cout << "x = " << X << endl << "Y = " << Y << endl << "A = " << A << endl << "B = " << B << endl << "C = " << C;
	return 0;
}