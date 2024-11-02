#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
		long long F, a, n, sum = 0, p = 0;
		cout << "������� a: ";
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
		cout << "������� n: ";
		while (true) {
			if (!(cin >> n)) { // ���� ����������� �����
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
		if (n <= 0) {
			cout << "ERROR" << endl;
		}
		for (long long i = 0; i <= n - 1; i++) {
			F = powl(2, i) * powl((-1), i) * a;
			sum = sum + F;
			if (F >= LLONG_MAX - powl(2, i - 1) * powl((-1), i - 1) * a || sum >= LLONG_MAX - powl(2, i - 1) * powl((-1), i - 1) * a) {
				p = 1;
				cout << "��� ������ ����� ���������� ������������" << endl;
				break;
			}
		}
		if (p != 1 && n > 0) {
			cout << "���� =  " << sum << endl;
		}
		cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
		cin >> c;
	}
	return 0;
}