#include <iostream>
using namespace std;
int main() {
	char c = '+';
	while (c == '+') {
		setlocale(LC_ALL, "RU");
		long long b, sum;
		float a;
		cout << "�� ������ ����� ����� ����������� ������������: ";
		while (true) {
			if (!(cin >> a)) { // ���� ����������� �����
				cin.clear(); //������� ���� ������ ��� cin (����� ������� �������� �����-������ �������� ���������)
				while (cin.get() != '\n'); //������ �� ������ ���� ������
				cout << "�������� ���� \n";
				continue;
			}
			if (cin.peek() != '\n') { //����������� �� ������� ����� � �������� ���, ����� ����� ��������� ������
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ������� ������� �� ������
				cout << "�������� ���� \n";
				continue;
			}
			break;
		}
		b = (int)a;
		if (b > 0) {
			if (b < 2) {
				cout << "C���� ������ ����� �����: " << 0 << endl;
			}
			else if (b >= 2) {
				if (b % 2 != 0) {
					b--;
				}
				sum = (b * b + 2 * b) / 4;
				cout << "C���� ������ ����� �����: " << sum << endl;
			}
		}
		else { cout << "���������� ������� �������� ������" << endl; }
		cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
		cin >> c;
	}
	return 0;
}