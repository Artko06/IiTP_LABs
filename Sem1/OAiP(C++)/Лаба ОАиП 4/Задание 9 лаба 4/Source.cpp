#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	int*** a = NULL;
	short N, s = 0;
	int d[4]{}, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0, d7 = 0, d8 = 0, d9 = 0, d10 = 0, d11 = 0, d12 = 0, d13 = 0, d14 = 0, d15 = 0, d16 = 0, max;
	cout << "��� 3-x ������ ������������ ������ �������� n^3 ����� ��������������� �����. ���������� ���������� ��������� � ���������� ������ �����\n";
	cout << "������� ������ 3-� ������� ������������� �������: ";
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
		if (N < 2) {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "�������� ���� \n";
			continue;
		}
		break;
	}
	a = (int***)malloc(N * sizeof(int**));
	for (int i = 0; i < N; i++) {
		a[i] = (int**)malloc(N * sizeof(int*));
		for (int j = 0; j < N; j++) {
			a[i][j] = (int*)malloc(N * sizeof(int));
		}
	}
	for (int i = 0; i < N; i++) {
		for (int q = 0; q < N; q++) {
			for (int w = 0; w < N; w++) {
				cout << i + 1 << " ������ " << q + 1 << " ������� " << w + 1 << " ���:\n";
				while (true) {
					if (!(cin >> a[i][q][w])) { // ���� ����������� �����
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
	}
	/*
	for (int p = 0; p < N; p++) {
		d[0] += a[p][p][0];
	}
	for (int p = 0, q =  N - 1; p < N, q >= 0; p++, q--) {
		d[1] += a[p][q][0];
	}
	for (int p = 0, q = N - 1; p < N, q >= 0; p++, q--) {
		d[2] += a[0][p][q];
	}
	for (int p = 0; p < N; p++) {
		d[3] += a[0][p][p];
	}
	for (int p = 0; p < N; p++) {
		d[4] += a[p][N - 1][p];
	}
	for (int p = 0, q = N - 1; p < N, q >= 0; p++, q--) {
		d[5] += a[p][N - 1][q];
	}
	for (int p = 0, q = N - 1; p < N, q >= 0; p++, q--) {
		d[6] += a[p][0][q];
	}
	for (int p = 0; p < N; p++) {
		d[7] += a[p][0][p];
	}
	for (int p = 0; p < N; p++) {
		d[8] += a[p][p][N - 1];
	}
	for (int p = 0, q = N - 1; p < N, q >= 0; p++, q--) {
		d[9] += a[p][q][N - 1];
	}
	for (int p = 0; p < N; p++) {
		d[10] += a[N - 1][p][p];
	}
	for (int p = 0, q = N - 1; p < N, q >= 0; p++, q--) {
		d[11] += a[N - 1][p][q];
	}
	*/
	for (int p = 0, q = N - 1; p < N, q >= 0; p++, q--) {
		d[0] += a[q][p][p];
	}
	for (int p = 0, q = N - 1; p < N, q >= 0; p++, q--) {
		d[1] += a[p][q][p];
	}
	for (int p = 0; p < N; p++) {
		d[2] += a[p][p][p];
	}
	for (int p = 0, q = N - 1; p < N, q >= 0; p++, q--) {
		d[3] += a[q][q][p];
	}
	max = d[0];
	for (int i = 0; i < 4; i++) {
		if (d[i] >= max) {
			max = d[i];
			s = i;
		}
	}
	cout << "��������� �" << s + 1 << " � ���������� ������ ����� ����� " << max << ' ';
	if (max == d[0] || max == d[1] || max == d[2] || max == d[3]) {
		cout << "� ��������� ������ ������������ �������\n";
	}
	else { cout << "� ��������� ������� ������������ �������\n"; }
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; ++j) {
			free(a[i][j]);
			a[i][j] == NULL;
		}
	for (int i = 0; i < N; i++) { 
		free(a[i]);
		a[i] = NULL;
	}
	free(a);
	a = NULL;
	cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> c;
	}
	return 0;
}