#include <iostream>
#include <string>


int main() {
	setlocale(LC_ALL, "RU");
	char replay = '+';
	while (replay == '+') {

	std::string dopcode;

	while (true) {
		bool codtop = 1;
		short symbol_tochka = 0;

		std::cout << "Введите дополнительный код: ";
		std::cin >> dopcode;

		for (int i = 0; i < dopcode.size(); i++) {
			if (dopcode.at(i) == '.') symbol_tochka++;
			if(dopcode.at(i) != '.' && dopcode.at(i) != '0' && dopcode.at(i) != '1') codtop = 0;
		}

		if (codtop == 0 || symbol_tochka > 1 || dopcode.size() == 1 || dopcode.at(dopcode.size() - 1) == '.'
			|| dopcode.at(0) == '.') {
			std::cout << "Неверный ввод \n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		else break;
	}

	if (dopcode.at(0) == '1') { // Отрицательное :(

		for (int i = dopcode.length() - 1; i > 0; i--) { // перевод из доп в обратный код
			if (dopcode.at(i) == '0') dopcode.at(i) = '1';
			else if (dopcode.at(i) == '1') {
				dopcode.at(i) = '0';
				break;
			}
		}
		 
		for (int i = 1; i < dopcode.length(); i++) { // перевод из обратки в прямой код
			if (dopcode.at(i) == '0') dopcode.at(i) = '1';
			else if (dopcode.at(i) == '1') dopcode.at(i) = '0';
		}
	}
	
	std::cout << "Из дополнительного в прямой код:\n" << dopcode;

	std::cout << "\nВведите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
	std::cin >> replay;
    }

	return 0;
}