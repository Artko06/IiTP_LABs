#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char d = '+';
	while (d == '+') {
	long long stra, strb, strc, m = 0, sum = 0;
	cout << "���������� ����� ������������ ������ A * B\n";
	cout << "������� ������ ������� �: ";
	while (true) {
		if (!(cin >> stra)) { // ���� ����������� �����
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
		if (stra < 1) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	strc = stra;
	int** a = new int*[stra];
	for (int i = 0; i < stra; i++) {
		a[i] = new int[stra];
	}
	for (int p = 0; p < stra; p++) {
		for (int i = 0; i < stra; i++) {
			cout << p + 1 << " ������ " << i + 1 << " �������: ";
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
	cout << "������� A:\n";
	for (int p = 0; p < stra; p++) {
		for (int i = 0; i < stra; i++) {
			cout << a[p][i] << '\t';
		}
		cout << '\n';
	}
	cout << "������� ������ ������� B: ";
	while (true) {
		if (!(cin >> strb)) { // ���� ����������� �����
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
		if (strb < 1 || strb != stra) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	int** b = new int* [strb];
	for (int i = 0; i < strb; i++) {
		b[i] = new int[strb];
	}
	for (int p = 0; p < strb; p++) {
		for (int i = 0; i < strb; i++) {
			cout << p + 1 << " ������ " << i + 1 << " �������: ";
			while (true) {
				if (!(cin >> b[p][i])) { // ���� ����������� �����
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
	cout << "������� B:\n";
	for (int p = 0; p < strb; p++) {
		for (int i = 0; i < strb; i++) {
			cout << b[p][i] << '\t';
		}
		cout << '\n';
	}
	int** c = new int* [strc];
	for (int i = 0; i < stra; i++) {
		c[i] = new int[strc];
	}
	for (int p = 0; p < strc; p++) {
		for (int i = 0; i < strc; i++) {
			for (; m < strc; m++) {
				c[p][i] = a[p][m] * b[m][i];
				sum = sum + c[p][i];
			}
			c[p][i] = sum;
			sum = 0;
			m = 0;
		}
	}
	cout << "������� C:\n";
	for (int p = 0; p < strc; p++) {
		for (int i = 0; i < strc; i++) {
				cout << c[p][i] << ' ';
			//cout << c[p][i] << '\t';
		}
		cout << '\n';
	}
	for (int i = 0; i < stra; i++) {
		delete[] a;
		a = nullptr;
	}
	delete a;
	a = nullptr;
	for (int i = 0; i < stra; i++) {
		delete[] b;
		b = nullptr;
	}
	delete b;
	b = nullptr;
	for (int i = 0; i < stra; i++) {
		delete[] c;
		c = nullptr;
	}
	delete c;
	c = nullptr;
	cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> d;
	}
	return 0;
}