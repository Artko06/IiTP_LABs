#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	int str = 0;
	int stlb = 0;
	short p = 0, Nmin = 0;
	cout << "������� ���-�a ��������� ��������� ������� ������� �:\n";
	cout << "������� ���-�� ����� �������: ";
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
	cout << "������� ���-�� �������� �������: ";
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
	for (int i = 0; i < str; i++) {
		a[i] = new int[stlb];
	}
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
	cout << "�������:\n";
	for (; p < str; p++) {
		for (int i = 0; i < stlb; i++) {
			cout << a[p][i] << '\t';
		}
		cout << '\n';
	}
	p = 0;

	if (str == 1) {
		for (; p < str; p++) {
			for (int i = 0; i < stlb; i++) {
				if (p == 0 && i == 0) {
					if (a[p][i] < a[p][i + 1]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == 0 && i == stlb - 1) {
					if (a[p][i] < a[p][i - 1]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else {
					if (a[p][i] < a[p][i - 1] && a[p][i] < a[p][i + 1]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
			}
		}
	}
	else if (stlb == 1) {
		for (; p < str; p++) {
			for (int i = 0; i < stlb; i++) {
				if (p == 0 && i == 0) {
					if (a[p][i] < a[p + 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == str - 1 && i == 0) {
					if (a[p][i] < a[p - 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p + 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
			}
		}
	}
	else {
		for (; p < str; p++) {
			for (int i = 0; i < stlb; i++) {
				if (p > 0 && p < str - 1 && i > 0 && i < stlb - 1) {
					if (a[p][i] < a[p][i + 1] && a[p][i] < a[p - 1][i] && a[p][i] < a[p + 1][i] && a[p][i] < a[p][i - 1]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == 0 && i == 0) {
					if (a[p][i] < a[p][i + 1] && a[p][i] < a[p + 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == str - 1 && i == stlb - 1) {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p][i - 1]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == 0 && i == stlb - 1) {
					if (a[p][i] < a[p][i - 1] && a[p][i] < a[p + 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == str - 1 && i == 0) {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p][i + 1]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (i == 0) {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p][i + 1] && a[p][i] < a[p + 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (i == stlb - 1) {
					if (a[p][i] < a[p - 1][i] && a[p][i] < a[p][i - 1] && a[p][i] < a[p + 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == 0) {
					if (a[p][i] < a[p][i - 1] && a[p][i] < a[p][i + 1] && a[p][i] < a[p + 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
				else if (p == str - 1) {
					if (a[p][i] < a[p][i - 1] && a[p][i] < a[p][i + 1] && a[p][i] < a[p - 1][i]) {
						cout << "������� " << p + 1 << " ������ " << i + 1 << " ������� - ��������� ������� = " << a[p][i] << '\n';
						Nmin++;
					}
				}
			}
		}
	}
	cout << "���������� ��������� ��������� ������� = " << Nmin << '\n';

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