#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	const short n = 4, m = 3;
	long long a[n][m], sum = 0;
	short p;
	cout << "����� ����� ��������� �������������� � ������(�� ������) �������\n";
	for (int p = 0; p < n; p++) {
		for (int i = 0; i < m; i++) {
			cout << p + 1 << " c����� " << i + 1 << " c������\n";
			while (true) {
				if (!(cin >> a[p][i])) { // ���� ����������� �����
					cin.clear(); //������� ���� ������ ��� cin (����� ������� �������� �����-������ �������� ���������)
					while (cin.get() != '\n'); // ������ �� ������ ���� ������
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
		}
		if ((p + 1) % 2 == 0) {
			for (int i = 0; i < m; i++) {
				sum = sum + a[p][i];
			}
		}
	} 
	cout << "����� ��������� � ������ ������� �������: " << sum << '\n';
	cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> c;
	}
	return 0;
}