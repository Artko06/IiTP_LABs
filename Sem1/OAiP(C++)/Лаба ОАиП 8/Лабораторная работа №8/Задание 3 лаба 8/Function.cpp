#include <iostream>
#include <fstream>
#include "..//..//Correct_input3/Correct_input3/Header.h"
#include "function.h"
#include "list.h"


list* generator_struct(const int amount) {
	list* info_storage = new list[amount];
	return info_storage;
}

int write_list(list* info_storage, const int amount, int i) {

	for (int copy = 0; i < amount; ++i) {
		std::cout << i + 1 << ". ВНИМАНИЕ, вы можете ввести также '-1', чтобы взять значения из заполненого поля"
			"(относится к каждому полю)\nВведите наименование товара или 0, чтобы завершить ввод: ";
		info_storage[i].setName(correctInput_char(info_storage[i].getName(), i, sizeof(info_storage[i].getName())));

		if (info_storage[i].getFirstName() == '0') {
			info_storage[i].setFirstName(' ');
			std::cout << "Размер списка был сокращён до " << i << " полей\n";
			return i;
		}

		else if (info_storage[i].getCheckMinys() == '-' && info_storage[i].getCheckOne() == '1') {
			std::cout << "Введите номер поля(начиная с 1) из которого хотите скопировать данные и вставить в это поле: ";
			copy = correctInput_copy(copy, i);

			memcpy(info_storage[i].getName(), info_storage[copy - 1].getName(), strlen(info_storage[copy - 1].getName()));
			info_storage[i].setXname(strlen(info_storage[copy - 1].getName()), '\0');
		}

		std::cout << "Введите дату поступления товара на склад(через enter день, месяц и год соответственно): ";
		info_storage[i].setDay(correctInput_day(info_storage[i].getDay(), i));

		if (info_storage[i].getDay() != -1) {
			info_storage[i].setMonth(correctInput_month(info_storage[i].getMonth(), i));
			info_storage[i].setYear(correctInput_year(info_storage[i].getYear(), i));
		}

		if (info_storage[i].getDay() == -1) {
			std::cout << "Введите номер поля(начиная с 1) из которого хотите скопировать данные и вставить в это"
				" поле: ";
			copy = correctInput_copy(copy, i);

			info_storage[i].setDay(info_storage[copy - 1].getDay());
			info_storage[i].setMonth(info_storage[copy - 1].getMonth());
			info_storage[i].setYear(info_storage[copy - 1].getYear());
		}

		std::cout << "Введите количество этого товара: ";
		info_storage[i].setNum(correctInput_num(info_storage[i].getNum(), i));

		if (info_storage[i].getNum() == -1) {
			std::cout << "Введите номер поля(начиная с 1) из которого хотите скопировать данные и вставить в это поле: ";
			copy = correctInput_copy(copy, i);

			info_storage[i].setNum(info_storage[copy - 1].getNum());
		}

		std::cout << "Введите стоимость этого товара(в белоруских рублях): ";
		info_storage[i].setCost(correctInput_cost(info_storage[i].getCost(), i));

		if (info_storage[i].getCost() == -1) {
			std::cout << "Введите номер поля(начиная с 1) из которого хотите скопировать данные и вставить в это поле: ";
			copy = correctInput_copy(copy, i);

			info_storage[i].setCost(info_storage[copy - 1].getCost());
		}
	}
	return i;
}

list* relloceted(list* info_storage, int& amount) {
	if (info_storage[amount - 1].getFirstName() != ' ') {
		int first_amount = amount;

		std::cout << "Введите число полей после довыделения памяти, которое вы хотите: ";
		correctInput_amount(amount, first_amount);
		list* new_info_storage = new list[amount];

		for (short i = 0; i < first_amount; ++i) {
			memcpy(new_info_storage[i].getName(), info_storage[i].getName(), strlen(info_storage[i].getName()));
			new_info_storage[i].setXname(strlen(info_storage[i].getName()), '\0');

			new_info_storage[i].setNum(info_storage[i].getNum());
			new_info_storage[i].setDay(info_storage[i].getDay());
			new_info_storage[i].setMonth(info_storage[i].getMonth());
			new_info_storage[i].setYear(info_storage[i].getYear());
			new_info_storage[i].setCost(info_storage[i].getCost());
		}

		delete[] info_storage;
		info_storage = nullptr;

		amount = write_list(new_info_storage, amount, first_amount);
		return new_info_storage;
	}
	else {
		std::cout << "Давай шуруй заполнять, то что создал ранее\n";
		return info_storage;
	}
}

list* redactor(list* info_storage, int amount, bool& file) {
	if (info_storage[0].getFirstName() == ' ') {
		std::cout << "Ты такой прикольный, сам хоть знаешь, что редактировать собрался?\n";
		return info_storage;
	}

	int num_field = 0, num_struct = 0;
	std::cout << "\nВыберите поле для редактирования используя цифры рядом с описаниями полей или 0"
		"для выхода из ф - ции:\n"
		<< "1. Наименование товара\n2. Дата поступления\n3. Количество этого товара\n4. Стоимость этого товара\n";

	num_field = correctInput_redactor_field(num_field);
	if (num_field == 0) return info_storage;

	std::cout << "Выберите номер структуры для редактирования, начиная с 1, или 0 для выхода из ф-ции: ";
	num_struct = correctInput_redactor_struct(num_struct, amount);
	if (num_struct == 0) return info_storage;

	switch (num_field) {
	case 1:
		std::cout << "Введите наименование товара " << num_struct << ": ";
		correctInput_char(info_storage[num_struct - 1].getName(), 0, sizeof(info_storage[num_struct - 1].getName()));
		break;

	case 2:
		std::cout << "Введите дату поступления товара на склад(через enter день, месяц и год соответственно): "
			<< num_struct << ": ";
		info_storage[num_struct - 1].setDay((correctInput_day(info_storage[num_struct - 1].getDay(), 0)));
		info_storage[num_struct - 1].setMonth((correctInput_month(info_storage[num_struct - 1].getMonth(), 0)));
		info_storage[num_struct - 1].setYear((correctInput_year(info_storage[num_struct - 1].getYear(), 0)));
		break;

	case 3:
		std::cout << "Введите количество этого товара " << num_struct << ": ";
		info_storage[num_struct - 1].setNum((correctInput_num(info_storage[num_struct - 1].getNum(), 0)));
		break;

	case 4:
		std::cout << "Введите стоимость этого товара " << num_struct << ": ";
		info_storage[num_struct - 1].setCost((correctInput_cost(info_storage[num_struct - 1].getCost(), 0)));
		break;
	}

	if (!file) return info_storage;

	std::fstream outputFile("outputFile.bin", std::ios::in | std::ios::out | std::ios::binary);

	for (int i = 0; i < amount; ++i) {
		if (num_struct - 1 == i) {
			outputFile.seekp(sizeof(list) * i, std::ios::beg);
			outputFile.write((char*)&info_storage[i], sizeof(list));
			break;
		}
	}

	return info_storage;
}

list* delete_field(list* info_storage, int& amount) {
	if (info_storage[0].getFirstName() == ' ') {
		std::cout << "Ну и что ты собрался удалять, заполнил бы хоть?\n";
		return info_storage;
	}

	int num = 0;
	std::cout << "Введите номер поля(начиная с 1), которое хотите удалить, или 0 для выхода из ф-ции: ";
	num = correctInput_delete_field(num, amount);

	if (num == 0) return info_storage;

	amount -= 1;

	list* new_info_storage = new list[amount];

	for (short i = 0, j = 0; j < amount; ++i, ++j) {
		if (i == num - 1) {
			--j;
			continue;
		}

		memcpy(new_info_storage[i].getName(), info_storage[i].getName(), strlen(info_storage[i].getName()));
		new_info_storage[i].setXname(strlen(info_storage[i].getName()), '\0');

		new_info_storage[i].setNum(info_storage[i].getNum());
		new_info_storage[i].setDay(info_storage[i].getDay());
		new_info_storage[i].setMonth(info_storage[i].getMonth());
		new_info_storage[i].setYear(info_storage[i].getYear());
		new_info_storage[i].setCost(info_storage[i].getCost());
	}
	delete[] info_storage;
	info_storage = nullptr;
	return new_info_storage;
}

void input_one(list* info_storage, int i) {
	std::cout << "\n__" << i + 1 << "__\n"
		<< "Наименование товара:  " << info_storage[i].getName() << "\n"

		<< "Дата поступление товара:  ";
	if (info_storage[i].getDay() < 10) std::cout << 0;
	std::cout << info_storage[i].getDay() << '.';
	if (info_storage[i].getMonth() < 10) std::cout << 0;
	std::cout << info_storage[i].getMonth() << '.' << info_storage[i].getYear() << "\n"

		<< "Количество этого товара:  " << info_storage[i].getNum() << "\n"

		<< "Стоимость этого товара:  " << info_storage[i].getCost() << "p" << "\n\n";
}

list* search(list* info_storage, int amount) {
	if (info_storage[0].getFirstName() == ' ') {
		std::cout << "Ну и какую структуру ты найти собрался?\n";
		return info_storage;
	}

	int num_field = 0;
	std::cout << "\nВыберите поле на основе которого будет производиться поиск, используя цифры рядом с описаниями полей,"
		" или 0 для выхода из ф-ции:\n"
		<< "1. Наименование товара\n2. Дата поступления\n3. Количество этого товара\n4. Стоимость этого товара\n";
	num_field = correctInput_redactor_field(num_field);
	if (num_field == 0) return info_storage;

	char search_name[1000];
	bool input = 0;
	float search_cost = 0;
	int search_day = 0, search_month = 0, search_year = 0, search_num = 0;

	switch (num_field) {
	case 1:
		std::cout << "Введите наименование товара: ";
		correctInput_char(search_name, 0, sizeof(search_name));

		for (int i = 0; i < amount; ++i) {
			if (!strcmp(search_name, info_storage[i].getName())) {
				input_one(info_storage, i);
				input = 1;
			}
		}
		if (!input) std::cout << "Структуру с этим элементом найти не удалось\n";

		break;

	case 2:
		std::cout << "Введите дату поступления товара: ";
		search_day = correctInput_day(search_day, 0);
		search_month = correctInput_month(search_month, 0);
		search_year = correctInput_year(search_year, 0);

		for (int i = 0; i < amount; ++i) {
			if (info_storage[i].getDay() == search_day && info_storage[i].getMonth() == search_month &&
				info_storage[i].getYear() == search_year) {
				input_one(info_storage, i);
				input = 1;
			}
		}
		if (!input) std::cout << "Структуру с этим элементом найти не удалось\n";

		break;

	case 3:
		std::cout << "Введите количество товара: ";
		search_num = correctInput_num(search_num, 0);

		for (int i = 0; i < amount; ++i) {
			if (info_storage[i].getNum() == search_num) {
				input_one(info_storage, i);
				input = 1;
			}
		}
		if (!input) std::cout << "Структуру с этим элементом найти не удалось\n";

		break;

	case 4:
		std::cout << "Введите стоимость товара: ";
		search_cost = correctInput_cost(search_cost, 0);

		for (int i = 0; i < amount; ++i) {
			if (info_storage[i].getCost() == search_cost) {
				input_one(info_storage, i);
				input = 1;
			}
		}
		if (!input) std::cout << "Структуру с этим элементом найти не удалось\n";

		break;
	}

	return info_storage;
}

void insertionSort(list*& info_storage, const int amount) {
	if (info_storage[0].getFirstName() == ' ')
		std::cout << "Это конечно круто, но что ты собрался сортировать?\n";

	for (int i = 1; i < amount; i++)
		for (int j = i; j > 0 && info_storage[j].getCost() > info_storage[j - 1].getCost(); j--)
			std::swap(info_storage[j], info_storage[j - 1]);

	std::cout << "Сортировка была произведена успешно\n";
}

void input(list* info_storage, const int amount) {
	if (info_storage[0].getFirstName() != ' ') {
		for (int i = 0; i < amount; ++i) {
			if (info_storage[i].getCost() <= 100.314) continue;
			std::cout << "\n__" << i + 1 << "__\n"
				<< "Наименование товара:  " << info_storage[i].getName() << "\n"

				<< "Дата поступление товара:  ";
			if (info_storage[i].getDay() < 10) std::cout << 0;
			std::cout << info_storage[i].getDay() << '.';
			if (info_storage[i].getMonth() < 10) std::cout << 0;
			std::cout << info_storage[i].getMonth() << '.' << info_storage[i].getYear() << "\n"

				<< "Количество этого товара:  " << info_storage[i].getNum() << "\n"

				<< "Стоимость этого товара:  " << info_storage[i].getCost() << "p" << "\n\n";
		}
	}

	else std::cout << "Ну и зачем? У тебя структура даже не заполнена\n";
}

void input_file(list* info_storage, const int amount, bool& file) {
	if (info_storage[0].getFirstName() == ' ') {
		std::cout << "Ну и зачем? У тебя структура даже не заполнена\n";
		return;
	}

	std::ofstream outputFile("outputFile.bin", std::ios::out | std::ios::binary);

	if (outputFile.is_open()) {

		outputFile.write((char*)info_storage, amount * sizeof(list));

		outputFile.close();

		std::cout << "Данные в файл записаны успешно!\n";
		file = 1;
	}

	else std::cout << "Ошибка открытия файла!\n";
}

void read_file(list* info_storage, const int amount, bool file) {
	if (!file) {
		std::cout << "Файл ещё не был открыт!\n";
		return;
	}

	std::ifstream outputFile("outputFile.bin", std::ios::binary);

	list* new_info_storage = new list[amount];
	for (int i = 0; i < amount; ++i)
		outputFile.read((char*)&new_info_storage[i], sizeof(list));

	input(new_info_storage, amount);

	delete[] new_info_storage;
	new_info_storage = nullptr;
}