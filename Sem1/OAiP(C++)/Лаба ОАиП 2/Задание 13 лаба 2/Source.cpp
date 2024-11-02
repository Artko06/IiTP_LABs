#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	long long K;
	cout << "Введите число грибов: ";
	cin >> K;
	if (K < 0) {
		cout << "Число найденых грибов должно быть положительным!!!";
	}
	else if (K % 100 == 11 || K % 100 == 12 || K % 100 == 13 || K % 100 == 14) {
		cout << "Мы нашли " << K << " грибов в лесу";
	}
	else if (K % 10 == 2 || K % 10 == 3 || K % 10 == 4) {
		cout << "Мы нашли " << K << " гриба в лесу";
	}
	else if (K % 10 == 1) {
		cout << "Мы нашли " << K << " гриб в лесу";
	}
	else { cout << "Мы нашли " << K << " грибов в лесу"; }
	return 0;
}