#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	long long num, s;
	short p, t, ch = 0, z = 0, m = 0;
	bool a1 = 1, a2 = 1, a3 = 1, a4 = 1, a5 = 1, a6 = 1, a7 = 1, a8 = 1, a9 = 1, a0 = 1;
	cout << "���� ����������� �����. ��������� ���������� ���������� ���� ��������� ���� � ����� � ����������, ����� ����� ����� � ������� ������������ ����� � �������� i\n";
	cout << "������� ����������� �����: ";
	while (true) {
		if (!(cin >> num)) { // ���� ����������� �����
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
		if (num < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	cout << "������� ������: ";
	while (true) {
		if (!(cin >> p)) { // ���� ����������� �����
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
		if (p < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	s = num;
	while (s != 0) {
		s = s / 10;
		ch++;
	}
	int* a = new int[ch];
	short i = 0;
	while (num != 0) {
		t = num % 10;
		a[i] = t;
		i = i + 1;
		num = num / 10;
	}
	for (int i = 0; i < ch; i++) {
		if (a[i] == 0 && a0) { a0 = 0; m++; }
		else if (a[i] == 1 && a1) { a1 = 0; m++; }
		else if (a[i] == 2 && a2) { a2 = 0; m++; }
		else if (a[i] == 3 && a3) { a3 = 0; m++; }
		else if (a[i] == 4 && a4) { a4 = 0; m++; }
		else if (a[i] == 5 && a5) { a5 = 0; m++; }
		else if (a[i] == 6 && a6) { a6 = 0; m++; }
		else if (a[i] == 7 && a7) { a7 = 0; m++; }
		else if (a[i] == 8 && a8) { a8 = 0; m++; }
		else if (a[i] == 9 && a9) { a9 = 0; m++; }
	}
	cout << "���������� ��������� ����:\n";
	cout << m << '\n';
	cout << "������ ���� �������� � ����� N:\n";
	for (int i = 0; i < ch; i++) {
		if (a[i] == a[ch - p]) {
			cout << i << ' ';
		}
	}
	delete[] a;
	a = nullptr;
	cout << "\n������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> c;
	}
	return 0;
}