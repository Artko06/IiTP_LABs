#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long double X, Y;
	cout << "Значения Х и Y соответсвенно: ";
	cin >> X >> Y;
	bool XY = X > Y, YX = Y > X;
	long double result = XY ? X : YX ? Y : X;
	cout << result;
	XY = XY ? X : XY;
	cout << sizeof(XY ? X : XY);
	return 0;
}