#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	int el, max, min, imin = 0, imax = 0, s = 0;
	cout << "���������� �������� ������ ��� ��������, ������� ��������� � ������� ����� ����������� � ������������ ����������(�� ������� ��). ���� ����� ������� �������� ������� ����� ������� ����� ��������� ����, �� ������� ��� ������� �������� �� �������\n";
	cout << "������� ���-�� ��������� �������: ";
	while (true) {
		if (!(cin >> el)) { // ���� ����������� �����
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
		if (el < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	int* a = new int[el];
	cout << "������� �������� ��������� �������:\n";
	for (int i = 0; i < el; i++) {
		cout << i << " ������� � �������:\n";
		while (true) {
			if (!(cin >> a[i])) { // ���� ����������� �����
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
	max = a[0];
	for (int i = 0; i < el; i++) {
		if (max < a[i]) { max = a[i]; imax = i; }
	}
	min = a[0];
	for (int i = 0; i < el; i++) {
		if (min > a[i]) { min = a[i]; imin = i; }
	}
	if (abs(imin - imax) > 1) {
		if (imin > imax) {
			while (imax + 1 < imin) {
				a[imax + 1] = 0;
				imax++;
			}
		}
		else if (imax > imin) {
			while (imin + 1 < imax) {
				a[imin + 1] = 0;
				imin++;
			}
		}
	}
	for (int i = 0; i < el; i++) {
		cout << a[i] << ' ';
		if (a[i] == 0) s++;
	}
	cout << '\n' << "����:\n";
	if (s > el / 2) {
		int* b = new int[el - s];
		int z = 0;
		for (int p = 0; p < el; p++) {
			if (a[p] == 0) continue;
			else {
				b[z] = a[p];
				z++;
			}
		}
		for (int i = 0; i < el - s; i++) {
			cout << b[i] << ' ';
		}
		delete[] b;
		b = nullptr;
	}
	delete[] a;
	a = nullptr;
	cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> c;
	}
	return 0;
}