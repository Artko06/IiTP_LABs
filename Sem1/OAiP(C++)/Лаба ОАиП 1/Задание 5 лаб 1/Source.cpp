#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	int n;
	long double a1, a100, d, S;
	cout << "������ ���� �������������� ���������� �1: ";
	cin >> a1;
	cout << "����� ���� �������������� ���������� �100: ";
	cin >> a100;
	cout << "��� ����� � ������� n: ";
	cin >> n;
	d = (a100 - a1) / 99;
	cout << "�������� �������������� ����������: " << d << endl;
	S = ((2 * a1 + d*(n + 10 - 1)) / 2) * (n+10);
	cout << "C���� ������ n+10 ������ �������������� ����������: " << S;
	return 0;
}