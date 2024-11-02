#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	long long N, w = 2, s = 3, z = 0;
	bool x = 1;
	cout << "���������� ������� ������ � ������� � ������� ����� ��������� �����, ��������� � ����������\n";
	cout << "������� N: ";
	while (true) {
		if (!(cin >> N)) { // ���� ����������� �����
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
		if (N < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	long long **a = new long long *[sqrt(N)];
	for (int i = 0; i < sqrt(N); i++) {
		a[i] = new long long[sqrt(N)];
	}
	for (int p = 0; p < sqrt(N); p++) {
		for (int i = 0; i < sqrt(N); i++) {
			a[p][i] = 0;
		}
	}
	a[0][0] = 1;
	a[0][1] = 2;
	for (int p = 0; p < sqrt(N); p++) {
		for (; w < sqrt(N), s <= N; w++) {
			if (!x) {
				a[0][w - 1] = s;
				s++;
			}
			if (a[z + 1][0] == 0) {
				p++;
				for (int q = w - 1; q >= 0; q--) {
					a[p][q] = s;
					if (q != 0) {
						if (a[p][q - 1] != 0) {
							p++; 
							q++;
						}
					}
					s++;
				}
				z++;
				p = 0;
				x = 0;
				continue;
			}
			s++;
		}
	}
	cout << "������������ �����:\n";
	for (int p = 0; p < sqrt(N); p++) {
		for (int i = 0; i < sqrt(N); i++) {
			if (a[p][i] == N) {
				cout << p << ' ' << i;
			}
		}
	}
	
	cout << '\n' << "�������:\n";
	for (int p = 0; p < sqrt(N); p++) {
		for (int i = 0; i < sqrt(N); i++) {
			cout << a[p][i] << '\t';
		}
		cout << '\n';
	}
	cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> c;
	}
	return 0;
}