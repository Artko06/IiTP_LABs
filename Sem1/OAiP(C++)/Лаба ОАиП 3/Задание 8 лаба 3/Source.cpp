#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char a = '+';
	while (a == '+') {
		unsigned long long x, n, sum = 0;
		short ost, N;
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
		for (long long i = 1; i <= n; i++) {
			x = i;
			N = log10(x) + 1;
			while (x > 0) {
				ost = x % 10;
				sum = sum + pow(ost, N);
				x = x / 10;
			}
			if (i == sum) {
				cout << sum << endl;
				sum = 0;
			}
			else { sum = 0; }
		}
		cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
		cin >> a;
	}
	return 0;
}