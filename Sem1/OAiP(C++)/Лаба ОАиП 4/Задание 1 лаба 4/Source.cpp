#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	char c = '+';
	while (c == '+') {
	const short k = 5;
	int a[k], sum = 0, sum2 = 0;
	double sr;
	cout << "C��� �������:\n";
	cout << "������ ���������� ����������� ������ �� k �����. ���������� ���-�� ��������� ������ ��������\n";
	for (int i = 0; i < k; i++) {
		cout << i << " ������� �������: ";
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
		sum = sum + a[i];
	}
	sr = sum / k;
	cout << "�������� ������ ��������: \n";
	for (int i = 0; i < k; i++) {
		if (a[i] > sr) {
			cout << a[i] << " ";
			sum2++;
		}
	}
	cout << '\n' << "����������: " << sum2 << '\n';
	cout << "������� '+', ���� ������ ��������� ��� ����� ������ ������, ����� ��������� ���������: ";
	cin >> c;
	}
	return 0;
}