#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	long long str, stlb, p = 0;
	cout << "��� �������� ������� � �������� n*m ��������� ������� � ������ �� �������, �������� ������� �������� ��������� ���������: ������� B[i][j] ����� ������������� �� ��������� ������� �, ������������� ����� � ���� ������� (i, j), ������� �������(i, j).��� ���� ���������, ��� ������� (1, 1) - ������� ����� ������� �������\n";
	cout << "������� ���-�� ����� � �������: ";
	while (true) {
		if (!(cin >> str)) { // ���� ����������� �����
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
		if (str < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	cout << "������� ���-�� �������� � �������: ";
	while (true) {
		if (!(cin >> stlb)) { // ���� ����������� �����
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
		if (stlb < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	int** a = new int* [str];
	int** b = new int* [str];
	for (int i = 0; i < str; i++) {
		a[i] = new int[stlb];
	}
	cout << "������� �:\n";
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			cout << p + 1 << " ������ " << i + 1 << " C�����: ";
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
	}
	p = 0;
	for (int i = 0; i < str; i++) {
		b[i] = new int[stlb];
	}
	cout << "������� �:\n";
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			cout << a[p][i] << '\t';
		}
		cout << '\n';
	}
	p = 0;

	for (int i = 0; i < str; i++) {
		int max = a[i][0];
		for (int j = 0; j < stlb; j++) {
			if (max < a[i][j]) max = a[i][j];
			b[i][j] = max;
		}
	}
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			if (p == str - 1 && i == stlb - 1) break;
			else if (p == str - 1) continue;
			if (b[p][i] > b[p + 1][i]) {
				for (int k = p + 1; k < str; k++) {
					if(b[k][i] < b[p][i]) b[k][i] = b[p][i];
				}
			}
		}
	}
	p = 0;
	cout << "������� B:\n";
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			cout << b[p][i] << '\t';
		}
		cout << '\n';
	}

	for (int i = 0; i < str; i++) {
		delete[] a[i];
		a[i] = nullptr;
	}
	delete[] a;
	a = nullptr;
	cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> c;
	}
	return 0;
}