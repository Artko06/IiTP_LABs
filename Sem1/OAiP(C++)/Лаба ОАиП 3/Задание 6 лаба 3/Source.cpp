#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char a = '+';
	while (a == '+') {
		float step, oblast, y, x = -1, r, v;
		cout << "����� �� ������� ����� ��������� ������� ������ �����: ";
		while (true) {
			if (!(cin >> oblast)) { // ���� ����������� �����
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
		if (oblast >= 1) {
			step = 2 / (oblast - 1);
			y = sin(x) + tan(x) - 1.0 / (1 + x * x);
			r = abs(y);
			while (x <= 1) {
				y = sin(x) + tan(x) - 1.0 / (1 + x * x);
				if (abs(y) <= r) {
					r = abs(y);
					v = x;
				}
				x = x + step;
			}
			cout << "������ ���������: " << v << endl;
		}
		else { cout << "���������� ������� �������� ������" << endl; }
		cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
		cin >> a;
	}
	return 0;
}