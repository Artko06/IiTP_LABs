#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	// ����� S, ���� a, v0, t
	long double a, v0, t, S;
	cout << "�������� ��������� �: ";
	cin >> a;
	cout << "�������� ��������� �������� v0: ";
	cin >> v0;
	cout << "�������� ������� t: ";
	cin >> t;
	S = v0 * t + a * t * t / 2;
	cout << "���������� ���������� ����� S: " << S;
	return 0;
}
