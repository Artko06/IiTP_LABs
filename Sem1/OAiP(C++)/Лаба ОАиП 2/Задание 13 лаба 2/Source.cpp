#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long long K;
	cout << "������� ����� ������: ";
	cin >> K;
	if (K < 0) {
		cout << "����� �������� ������ ������ ���� �������������!!!";
	}
	else if (K % 100 == 11 || K % 100 == 12 || K % 100 == 13 || K % 100 == 14) {
		cout << "�� ����� " << K << " ������ � ����";
	}
	else if (K % 10 == 2 || K % 10 == 3 || K % 10 == 4) {
		cout << "�� ����� " << K << " ����� � ����";
	}
	else if (K % 10 == 1) {
		cout << "�� ����� " << K << " ���� � ����";
	}
	else { cout << "�� ����� " << K << " ������ � ����"; }
	return 0;
}