#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	int i = 1;
	long double dn, sum = 0;
	do {
		dn = 1.0 / (pow(2, i)) + 1.0 / (pow(3, i));
		sum = sum + dn;
		i++;
	} while (dn >= 0.001);
	cout << "Cумма ряда: " << endl << sum;
	return 0;
}