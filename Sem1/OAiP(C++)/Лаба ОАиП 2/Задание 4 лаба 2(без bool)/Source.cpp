#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double X, Y;
	cout << "�������� � � Y �������������: ";
	cin >> X >> Y;
	float result = X > Y ? X : Y > X ? Y : X;
	cout << result;
	return 0;
}