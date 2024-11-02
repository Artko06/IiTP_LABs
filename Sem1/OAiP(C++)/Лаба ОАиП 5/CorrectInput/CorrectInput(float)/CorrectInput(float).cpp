#include <iostream>

float main();

float correctInput_R(std::string& stroka, float& new_Num) {
	if (stroka.length() > 9) {
		std::cout << "Некоректный ввод\n";
		return main();
	}
	for (int i = 0; i < stroka.length(); i++) {
		if (stroka[i] >= '0' && stroka[i] <= '9') {
			if (i == stroka.length() - 1) {
				for (int i = stroka.length() - 1, iter = 1; i >= 0, iter < pow(10, stroka.length()); i--, iter *= 10) {
					new_Num += (stroka[i] - 48) * iter;
				}
				return new_Num;
			}
		}
		else {
			std::cout << "Некоректный ввод\n";
			return main();
		}
	}
}
float main() {
	setlocale(LC_ALL, "RU");
	std::string stroka;
	float new_Num = 0;
	std::cout << "Введите целое число: ";
	std::cin >> stroka;
	correctInput_R(stroka, new_Num);
	if (new_Num == 0) { return 0; }
	std::cout << new_Num;
	return 0;
}