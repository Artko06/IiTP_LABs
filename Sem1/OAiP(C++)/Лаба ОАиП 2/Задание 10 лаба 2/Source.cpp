#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double x1, y1, x2, y2, r, R, d;
	cout << "������� ���������� ������ 1-��� ����� x1 � y1 �������������: ";
	cin >> x1 >> y1;
	cout << "������ ������� �����: ";
	cin >> r;
	cout << "������� ���������� ������ 2-��� ����� x2 � y2 �������������: ";
	cin >> x2 >> y2;
	cout << "������ ������� �����: ";
	cin >> R;
	d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	if (d > R + r) {
		cout << "�� ���� ������� �� ���������" << endl; // ����� �� ������������
	}
	else if (r > R) {
		if (d + R < r) {
			cout << "��, �� ����������� �������� ��� �����" << endl;; // ���� 1 ������ � ���� 2
		}
		else if (d + R >= r) {
			cout << "������ ������������" << endl;;
		}
	}
	else if (R > r) {
		if (d + r < R) {
			cout << "��" << endl;; // ���� 2 ������ � ���� 1
		}
		else if (d + r >= R) {
			cout << "������ ������������" << endl;
		}
	}
	else if (R == r) {
		cout << "������ ������������" << endl;
	}
	return 0;
}