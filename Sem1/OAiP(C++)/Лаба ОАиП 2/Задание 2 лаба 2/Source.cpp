#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double x, y, z;
	cout << "������ ������� �����: ";
	cin >> x;
	cout << "������ ������� �����: ";
	cin >> y;
	cout << "������ ������� �����: ";
	cin >> z;
	if (x <= 0 || y <= 0 || z <= 0) {
		cout << "ERROR";
	}
	else if (x + y <= z || x + z <= y || y + z <= x) {
		cout << "����� ����������� �� ����������";
	}
	else { cout << "����� ����������� ����������"; 
	}
	return 0;
}
