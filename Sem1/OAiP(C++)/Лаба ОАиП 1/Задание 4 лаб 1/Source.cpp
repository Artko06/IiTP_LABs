#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
    int n;
	long double b1, q, S;
	cout << "�������� b1: ";
	cin >> b1;
	cout << "��� ����� � �������: ";
	cin >> n;
	q = 1.0 / (n + 1);
	S = b1 / (1 - q);
	cout << "����� ���������� ��������� �������������� ����������: " << S;
	return 0;
}