#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double S, P, Q, R, Qrad, r;
	const double PI = acos(-1);
	cout << "������ ����: ";
	cin >> R;
	cout << "����� ������� ������������ ���������: ";
	cin >> P;
	cout << "������ ���� ����� � ��������: ";
	cin >> Q;
	Qrad = (PI * Q) / 180;
	S = P * P * sin(Qrad);
	r = S / (2 * P);
	if (R <= 0 || P <= 0 || Q <= 0 || Q >= 90) {
		cout << "���� ������ ��������� ����������";
	}
	else if (R <= r) {
		cout << "��� ������ ����� ����������� ���������";
	}
	else { cout << "��� �� ������ ����� ����������� ���������"; }
}