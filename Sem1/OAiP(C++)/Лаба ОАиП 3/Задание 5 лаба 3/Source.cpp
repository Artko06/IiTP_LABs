#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char a = '+';
	while (a == '+') {
		long double F, n, sum = 0, ln, N, p = 1, x;
		cout << "�������� n: ";
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
		cout << "���-�� ���������: ";
		while (true) {
			if (!(cin >> N)) { // ���� ����������� �����
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
		if (N >= 1 && n >= 1) {
			while (p <= N) {
				cout << "������� � � ���������� �� 0.1 �� 1: ";
				cin >> x;
				if (x >= 0.1 && x <= 1) {
					int i = 1;
					while (i <= n) {
						F = powl((-1), i + 1) * powl(-(x * x + 2 * x + 1) / (x * x + 2 * x + 2), i) / i;
						sum = sum + F;
						i++;
					}
					cout << "���� ����: " << sum << endl;
					sum = 0;
					ln = log(1.0 / (2 + 2 * x + x * x));
					cout << "���� ln: " << ln << endl;
					p++;
				}
				else {
					cout << "�������� ������ ����������" << endl;
					break;
				}
			}
		}
		else { cout << "�������� ������ ����������" << endl; }
		cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
		cin >> a;
	}
	return 0;
}