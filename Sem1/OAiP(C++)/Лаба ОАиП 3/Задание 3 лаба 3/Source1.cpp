#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char a = '+';
	while (a == '+') {
		double H = acos(-1) / 40, y;
		float x;
		while (true) {
			cout << "������� ���: ";
			if (!(cin >> x)) { // ���� ����������� �����
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
		for (long double i = 10; i <= 20; i = i + x) {
			y = sqrt(2) * sin(i * H - 10 * H);
			cout << "�������� �-���: \n" << y << endl;
		}
		cout << "������� '+', ���� ������� ��������� ��� ����� ������ ������ ��� ���������� ���������: ";
		cin >> a;
	}
	return 0;
}