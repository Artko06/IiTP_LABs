#include <iostream>

int correctInput_choise(int& num) {
	while (true) {
		if (!(scanf_s("%d", &num))) {
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

		if (!(num >= 0 && num <= 7)) {
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
		if (!(scanf_s("%d", &num))) {
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
		if (!(scanf_s("%d", &num))) {
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
		if (!(scanf_s("%d", &num))) {
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

int correctInput_hour(int& num, int i) {
	while (true) {
		if (!(scanf_s("%d", &num))) {
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

		if (num < -1 || num > 23) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (num == -1 && i == 0) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

int correctInput_min(int& num) {
	while (true) {
		if (!(scanf_s("%d", &num))) {
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

		if (num > 59 || num < 0) {
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

		int num_space = 0;
		for (int i = 0; i < strlen(arr); ++i)
			if (arr[i] == ' ') num_space++;

		if (arr[0] == '-' && arr[1] == '1' && i == 0) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Гений, откуда ты копировать собрался? \n");
			continue;
		}

		if (num_space > 1) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}
	
	return arr;
}

int correctInput_freelot(int& num, int i) {
	while (true) {
		if (!(scanf_s("%d", &num))) {
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

		if (num < -1) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		if (num == -1 && i == 0) {
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			printf("Неверный ввод \n");
			continue;
		}

		break;
	}

	return num;
}

int correctInput_delete_field(int& num, int amount) {
	while (true) {
		if (!(scanf_s("%d", &num))) {
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
		if (!(scanf_s("%d", &num))) {
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
		if (!(scanf_s("%d", &num))) {
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