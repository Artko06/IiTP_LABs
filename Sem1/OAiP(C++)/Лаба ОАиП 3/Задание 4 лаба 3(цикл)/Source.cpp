#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
		long long sum = 0;
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
		a = (int)a;
		for (long long i = 1; i <= a; i++, cout << 1) {
			if (i % 2 != 0) {
				continue;
			}
			else if (i % 2 == 0) {
				sum = sum + i;
			}
		}
		cout << "����� ���� ������ �����: " << sum << endl;
		cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
		cin >> c;
	}
	return 0;
}