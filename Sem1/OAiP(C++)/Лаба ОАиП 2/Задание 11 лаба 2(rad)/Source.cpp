#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double S, P, Q, R, r;
	const double PI = acos(-1);
	cout << "������ ����: ";
	cin >> R;
	cout << "����� ������� ������������ ���������: ";
	cin >> P;
	cout << "������ ���� ����� � ��������: ";
	cin >> Q;
	S = P * P * sin(Q);
	r = S / (2 * P);
	if (R <= 0 || P <= 0 || Q <= 0 || Q >= PI/2) {
		cout << "���� ������ ��������� ����������";
	}
	else if (R <= r) {
		cout << "��� ������ ����� ����������� ���������";
	}
	else { cout << "��� �� ������ ����� ����������� ���������"; }
}