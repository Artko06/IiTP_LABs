#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	int x, y;
	cout << "�������� � ����������: " << endl;
	cin >> x;
	cout << "�������� y ����������: " << endl;
	cin >> y;
	cout << "�������� x � y ������������� �� 1-�� ����: " << x << '\t' << y << endl;
	if (x-y < INT_MIN || x-y > INT_MAX) {
		cout << "�������� ������������ �����";
	}
	x = x - y;
	cout << "�������� x � y ������������� ����� 1-�� ����: " << x << '\t' << y << endl;
	if (y + x < INT_MIN || y + x > INT_MAX) {
		cout << "�������� ������������ �����";
	}
	y = y + x; // y ���� ��������� �
	if (x - y < INT_MIN || x - y > INT_MAX) {
		cout << "�������� ������������ �����";
	}
	cout << "�������� x � y ������������� ����� 2-�� ����: " << x << '\t' << y << endl;
	x = x - y;
	cout << "�������� x � y ������������� ����� 3-�� ����: " << x << '\t' << y << endl;
	x = -x;
	cout << "���� �: " << x << endl;
	cout << "���� y: " << y << endl;
	return 0;
}