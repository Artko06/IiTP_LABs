#include <iostream>
using namespace std;
int main() {
    setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
    long long N;
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
    long long i = sqrt(N);
    if (sqrt(N) != i) { i++; }
    long long p = i * i - (i - 1);
    cout << "������������ �����:\n";
    if (N == p) {
        cout << i - 1 << ' ' << i - 1;
    }
    else if (p > N) {
        cout << ((i - 1) - (p - N)) << ' ' << i - 1;
    }
    else {
        cout << i - 1 << ' ' << ((i - 1) - (N - p));
    }
	cout << "\n������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> c;
	}
    return 0;
}