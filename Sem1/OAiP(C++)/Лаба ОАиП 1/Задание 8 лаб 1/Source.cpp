#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	int h1, h2, min1, min2, Xmin, Xh, Xmin2;
	cout << "�������� ����� � �����, ����� ������� ����� ������ ������, ��������������: " << endl;
	cin >> h1 >> min1;
	cout << "�������� ����� � �����, ����� ������� �������� ������ ������, ��������������: " << endl;
	cin >> h2 >> min2;
	if (h2 < h1) {
		Xmin = (1440 + (h2 * 60 + min2)) - (h1 * 60 + min1); // ����� ����� � �������
	}
	else if (h2 == h1 && min1>min2) {
		Xmin = (1440 + (h2 * 60 + min2)) - (h1 * 60 + min1); // ����� ����� � �������
	}
	else {
		Xmin = (h2 * 60 + min2) - (h1 * 60 + min1);
	} //����� ����� � �������
	Xh = Xmin / 60; 
	Xmin2 = Xmin % 60;
	cout << "�� ������� ����� ������� ��������: " << Xh << "� � " << Xmin2 << "���";
	return 0;
}