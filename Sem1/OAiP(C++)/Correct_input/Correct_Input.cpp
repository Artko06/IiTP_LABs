#include <iostream>
long long correctInput_LL(long long& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	return num;
}
double correctInput_D(double& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	return num;
}
long double correctInput_LD(long double& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	return num;
}
float correctInput_F(float& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	return num;
}
short correctInput_S(short& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	return num;
}
int correctInput_I(int& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	return num;
}
short correctInput_S_N(short& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		if (num < 1) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Неверный ввод \n";
			continue;
		}
		break;
	}
	return num;
}