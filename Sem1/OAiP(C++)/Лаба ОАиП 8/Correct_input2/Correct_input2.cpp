#include <iostream>

int correctInput_choise(int& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (num > 9 || num < 0) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

int correctInput_amount(int& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (num < 0) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

int correctInput_amount(int& num, int first_amount) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (num < first_amount) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

int correctInput_copy(int& num, int i) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (!(num > 0 && num < i + 1)) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

char* correctInput_char(char* arr, int i, int size) {
	while (true) {
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		scanf_s("%[^\n]", arr, size - 1);

		if (arr[0] == '-' && arr[1] == '1' && i == 0) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Гений, откуда ты копировать собрался? \n");
			continue;
		}

		break;
	}

	return arr;
}

int correctInput_delete_field(int& num, int amount) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (num < 0 || num > amount) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

int correctInput_redactor_field(int& num) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (num < 0 || num > 5) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

int correctInput_redactor_struct(int& num, int amount) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (num < 0 || num > amount) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

float correctInput_mark(float& num, int i) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (i != 0 && num == -1) {
			break;
		}

		if (num < 0 || num > 10){
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}


		break;
	}

	return num;
}

float correctInput_profit(float& num, int i) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (i != 0 && num == -1) {
			break;
		}

		if (num < 0) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

int correctInput_num_family(int& num, int i) {
	while (true) {
		if (!(std::cin >> num)) {
			std::cin.clear();
			while (std::cin.get() != '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (std::cin.peek() != '\n') {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (i != 0 && num == -1) {
			break;
		}

		if (num <= 0) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}