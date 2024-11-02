#include <iostream>

int main();

int correctInput_N(std::string& stroka, int& new_Num) {
	if (stroka.length() > 9) {
		std::cout << "Некоректный ввод\n";
		return main();
	}
	else if (stroka.length() == 1 && stroka[0] == 0) {
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

int correctInput_Z(std::string& stroka, int& new_Num) {
	if (stroka.length() > 9) {
		std::cout << "Некоректный ввод\n";
		return main();
	}
	for (int i = 0; i < stroka.length(); i++) {
		if (i == 0 && stroka[i] == '-') { i++; }
		else if (stroka[i] >= '0' && stroka[i] <= '9') {
			if (i == stroka.length() - 1 && stroka[0] == '-') {
				for (int i = stroka.length() - 1, iter = 1; i > 0, iter < pow(10, stroka.length() - 1); i--, iter *= 10) {
					new_Num += (stroka[i] - 48) * iter;
				}
				new_Num *= (-1);
			}
			else {
				if (i == stroka.length() - 1) {
					for (int i = stroka.length() - 1, iter = 1; i >= 0, iter < pow(10, stroka.length()); i--, iter *= 10) {
						new_Num += (stroka[i] - 48) * iter;
					}
				}
			}
		}
		else {
			std::cout << "Некоректный ввод\n";
			return main();
		}
	}
	return new_Num;
}

int main() {
	setlocale(LC_ALL, "RU");
	std::string stroka;
	int new_Num = 0;
	std::cout << "Введите целое число: ";
	std::cin >> stroka;
	correctInput_Z(stroka, new_Num);
	if (new_Num == 0) { return 0; }
	std::cout << new_Num;
	return 0;
}