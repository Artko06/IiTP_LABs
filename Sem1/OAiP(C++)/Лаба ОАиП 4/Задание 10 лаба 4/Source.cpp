#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char d = '+';
	while (d == '+') {
	long long N, s, num, num2, s2;
	short t = 0, q = 0, ch = 0, ch2 = 0, nylik = 0;
	cout << "� ���� � ��������� ����� ���� 1 �������. ���� ��������� ��� �������������, ���� ����� N ����������� �����, ��� i - � ���������� ����� a[i] ������.���������, ������� ������� ��������� � ����, ���� �� ���������� �������� ���������� ?\n";
	cout << "���������� �������� �����������: ";
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
		if (N < 0) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	if (N % 2 == 0) {
		s = N / 2;
		num = N / 2;
		num2 = N - 1;
		s2 = N - 1;
	}
	else {
		s = N;
		num = N;
		num2 = (N - 1) / 2;
		s2 = (N - 1) / 2;
	}
	while (s != 0) {
		s = s / 10;
		ch++;
	}
	int* a = new int[ch];
	while (num != 0) {
		t = num % 10;
		a[q] = t;
		q++;
		num = num / 10;
	}
	q = 0;
	t = 0;
	while (s2 != 0) {
		s2 = s2 / 10;
		ch2++;
	}
	int* b = new int[ch2];
	while (num2 != 0) {
		t = num2 % 10;
		b[q] = t;
		q++;
		num2 = num2 / 10;
	}
	q = 0;
	t = 0;
	int* c = new int[ch + ch2]; 
	for (int i = 0; i < ch + ch2; i++) {
		c[i] = 0;
	}
	for (int i = 0; i < ch; i++) {
		for (int j = 0; j < ch2; j++) {
			c[i + j] += a[i] * b[j];
			c[i + j + 1] = c[i + j + 1] + c[i + j] / 10;
			c[i + j] = c[i + j] % 10;
		}
	}
	for (int i = 0; i < ch + ch2; i++) {
		if (c[i] == 9) {
			nylik++;
		}
		else if (c[i] != 9) {
			break;
		}
	}
	if (nylik == 0) {
		c[0]++;
	}
	for (int i = 0; i < nylik; i++) {
		c[nylik - 1]++;
		c[i] == 0;
	}
	cout << "���������� �������:\n";
	if (c[ch + ch2 - 1] == 0) {
		for (int i = ch + ch2 - 2; i >= 0; i--) {
			cout << c[i];
		}
	}
	else {
		for (int i = ch + ch2 - 1; i >= 0; i--) {
			cout << c[i];
		}
	}
	delete[] a;
	a = nullptr;
	delete[] b;
	b = nullptr;
	delete[] c;
	c = nullptr;
	cout << "\n������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> d;
	}
	return 0;
}