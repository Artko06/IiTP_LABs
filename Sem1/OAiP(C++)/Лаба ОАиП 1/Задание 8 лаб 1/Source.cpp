#include <iostream>
using namespace std;
int main() {
	setlocale(LC_ALL, "RU");
	int h1, h2, min1, min2, Xmin, Xh, Xmin2;
	cout << "Значение часов и минут, когда студент начал решать задачи, соответственно: " << endl;
	cin >> h1 >> min1;
	cout << "Значение часов и минут, когда студент закончил решать задачи, соответственно: " << endl;
	cin >> h2 >> min2;
	if (h2 < h1) {
		Xmin = (1440 + (h2 * 60 + min2)) - (h1 * 60 + min1); // всего решал в минутах
	}
	else if (h2 == h1 && min1>min2) {
		Xmin = (1440 + (h2 * 60 + min2)) - (h1 * 60 + min1); // всего решал в минутах
	}
	else {
		Xmin = (h2 * 60 + min2) - (h1 * 60 + min1);
	} //Всего решал в минутах
	Xh = Xmin / 60; 
	Xmin2 = Xmin % 60;
	cout << "На решение задач студент потратил: " << Xh << "ч и " << Xmin2 << "мин";
	return 0;
}