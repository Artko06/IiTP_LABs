#include <iostream>
#include <fstream>
#include "..//..//Correct_input2/Correct_input2/Header.h"
#include "function.h"
#include "list.h"


struct list* generator_struct(const int amount) {
	list* info_student = new list[amount];
	return info_student;
}

int write_list(list* info_student, const int amount, int i) {

	for (int copy; i < amount; ++i) {
		std::cout << i + 1 << ". ��������, �� ������ ������ ����� '-1', ����� ����� �������� �� ����������� ����"
			"(��������� � ������� ����)\n������� �.�.�. c������� ��� 0, ����� ��������� ����: ";
		correctInput_char(info_student[i].name, i, sizeof(info_student[i].name));

		if (info_student[i].name[0] == '0') {
			info_student[i].name[0] = ' ';
			std::cout << "������ ������ ��� �������� �� " << i << " �����\n";
			return i;
		}

		else if (info_student[i].name[0] == '-' && info_student[i].name[1] == '1') {
			std::cout << "������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ";
			correctInput_copy(copy, i);

			memcpy(info_student[i].name, info_student[copy - 1].name, strlen(info_student[copy - 1].name));
			info_student[i].name[strlen(info_student[copy - 1].name)] = '\0';
		}

		std::cout << "������� ����� ������ ��������: ";
		correctInput_char(info_student[i].number_group, i, sizeof(info_student[i].number_group));

		if (info_student[i].number_group[0] == '-' && info_student[i].number_group[1] == '1') {
			std::cout << "������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ";
			correctInput_copy(copy, i);

			memcpy(info_student[i].number_group, info_student[copy - 1].number_group, strlen(info_student[copy - 1].number_group));
			info_student[i].number_group[strlen(info_student[copy - 1].number_group)] = '\0';
		}

		std::cout << "������� ������� ���� ��������(�� � �������� ���� ����): ";
		correctInput_mark(info_student[i].mark, i);

		if (info_student[i].mark == -1) {
			std::cout << "������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ";
			correctInput_copy(copy, i);

			info_student[i].mark = info_student[copy - 1].mark;
		}

		std::cout << "������� ���������� ������ �����: ";
		correctInput_num_family(info_student[i].num_family, i);

		if (info_student[i].num_family == -1) {
			std::cout << "������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ";
			correctInput_copy(copy, i);

			info_student[i].num_family = info_student[copy - 1].num_family;
		}

		std::cout << "������� ������� ����� ����� ��������(� ���������� ������): ";
		correctInput_profit(info_student[i].profit_family, i);

		if (info_student[i].profit_family == -1) {
			std::cout << "������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ";
			correctInput_copy(copy, i);

			info_student[i].profit_family = info_student[copy - 1].profit_family;
		}
	}
	return i;
}

struct list* relloceted(list* info_student, int& amount) {
	if (info_student[amount - 1].name[0] != ' ') {
		int first_amount = amount;

		std::cout << "������� ����� ����� ����� ����������� ������, ������� �� ������: ";
		correctInput_amount(amount, first_amount);
		list* new_info_student = new list[amount];

		for (short i = 0; i < first_amount; ++i) {
			memcpy(new_info_student[i].name, info_student[i].name, strlen(info_student[i].name));
			new_info_student[i].name[strlen(info_student[i].name)] = '\0';
			memcpy(new_info_student[i].number_group, info_student[i].number_group, strlen(info_student[i].number_group));
			new_info_student[i].number_group[strlen(info_student[i].number_group)] = '\0';

			new_info_student[i].num_family = info_student[i].num_family;
			new_info_student[i].mark = info_student[i].mark;
			new_info_student[i].profit_family = info_student[i].profit_family;
		}

		delete[] info_student;
		info_student = nullptr;

		amount = write_list(new_info_student, amount, first_amount);
		return new_info_student;
	}
	else {
		std::cout << "����� ����� ���������, �� ��� ������ �����\n";
		return info_student;
	}
}

struct list* redactor(list* info_student, int amount) {
	if (info_student[0].name[0] == ' ') {
		std::cout << "�� ����� ����������, ��� ���� ������, ��� ������������� ��������?\n";
		return info_student;
	}

	int num_field, num_struct;
	std::cout << "\n�������� ���� ��� �������������� ��������� ����� ����� � ���������� ����� ��� 0"
		"��� ������ �� � - ���:\n"
		<< "1. �.�.� ��������\n2. ����� ������ ��������\n3. ������� ���� ��������\n4. ���������� ������ �����\n"
		"5. ����� ����� ��������\n";
	correctInput_redactor_field(num_field);
	if (num_field == 0) return info_student;

	std::cout << "�������� ����� ��������� ��� ��������������, ������� � 1, ��� 0 ��� ������ �� �-���: ";
	correctInput_redactor_struct(num_struct, amount);
	if (num_struct == 0) return info_student;

	switch (num_field) {
	case 1:
		std::cout << "������� �.�.� �������� " << num_struct << ": ";
		correctInput_char(info_student[num_struct - 1].name, 0, sizeof(info_student[num_struct - 1].name));
		break;

	case 2:
		std::cout << "������� ����� ������ �������� " << num_struct << ": ";
		correctInput_char(info_student[num_struct - 1].number_group, 0, sizeof(info_student[num_struct - 1].number_group));
		break;

	case 3:
		std::cout << "������� c������ ���� ��������(�� � �������� ����) " << num_struct << ": ";
		correctInput_mark(info_student[num_struct - 1].mark, 0);
		break;

	case 4:
		std::cout << "������� ���������� ������ ����� " << num_struct << ": ";
		correctInput_num_family(info_student[num_struct - 1].num_family, 0);
		break;


	case 5:
		std::cout << "������� ����� ����� ��������" << num_struct << ": ";
		correctInput_profit(info_student[num_struct - 1].profit_family, 0);
		break;

	}

	return info_student;
}

struct list* delete_field(list* info_student, int& amount) {
	if (info_student[0].name[0] == ' ') {
		std::cout << "�� � ��� �� �������� �������, �������� �� ����?\n";
		return info_student;
	}

	int num;
	std::cout << "������� ����� ����(������� � 1), ������� ������ �������, ��� 0 ��� ������ �� �-���: ";
	correctInput_delete_field(num, amount);

	if (num == 0) return info_student;

	amount -= 1;

	list* new_info_student = new list[amount];

	for (short i = 0, j = 0; j < amount; ++i, ++j) {
		if (i == num - 1) {
			--j;
			continue;
		}

		memcpy(new_info_student[j].name, info_student[i].name, strlen(info_student[i].name));
		new_info_student[j].name[strlen(info_student[i].name)] = '\0';
		memcpy(new_info_student[j].number_group, info_student[i].number_group, strlen(info_student[i].number_group));
		new_info_student[j].number_group[strlen(info_student[i].number_group)] = '\0';

		new_info_student[j].num_family = info_student[i].num_family;
		new_info_student[j].mark = info_student[i].mark;
		new_info_student[j].profit_family = info_student[i].profit_family;
	}
	delete[] info_student;
	info_student = nullptr;
	return new_info_student;
}

void input_one(list* info_student, int i) {
	std::cout << "\n__" << i + 1 << "__\n"
		<< "�.�.� ��������:  " << info_student[i].name << "\n"
		<< "����� ������ ��������:  " << info_student[i].number_group << "\n"

		<< "C������ ���� ��������:  " << info_student[i].mark << "\n"

		<< "���������� ������ �����:  " << info_student[i].num_family << "\n"

		<< "����� ����� ��������:  " << info_student[i].profit_family << "p" << "\n\n";
}

struct list* search(list* info_student, int amount) {
	if (info_student[0].name[0] == ' ') {
		std::cout << "�� � ����� ��������� �� ����� ��������?\n";
		return info_student;
	}

	int num_field;
	std::cout << "\n�������� ���� �� ������ �������� ����� ������������� �����, ��������� ����� ����� � ���������� �����,"
		" ��� 0 ��� ������ �� �-���:\n"
		<< "1. �.�.� ��������\n2. ����� ������ ��������\n3. ������� ���� ��������\n4. ���������� ������ �����\n"
		"5. ����� ����� ��������\n";
	correctInput_redactor_field(num_field);
	if (num_field == 0) return info_student;

	char search_name[1000], search_group[100];
	bool input = 0;
	float mark, profit_family;
	int num_family;

	switch (num_field) {
	case 1:
		std::cout << "������� �.�.� ��������: ";
		correctInput_char(search_name, 0, sizeof(search_name));

		for (int i = 0; i < amount; ++i) {
			if (!strcmp(search_name, info_student[i].name)) {
				input_one(info_student, i);
				input = 1;
			}
		}
		if (!input) std::cout << "��������� � ���� ��������� ����� �� �������\n";

		break;

	case 2:
		std::cout << "������� ����� ������ ��������: ";
		correctInput_char(search_group, 0, sizeof(search_group));

		for (int i = 0; i < amount; ++i) {
			if (!strcmp(search_group, info_student[i].number_group)) {
				input_one(info_student, i);
				input = 1;
			}
		}
		if (!input) std::cout << "��������� � ���� ��������� ����� �� �������\n";

		break;

	case 3:
		std::cout << "������� ������� ���� ��������: ";
		correctInput_mark(mark, 0);

		for (int i = 0; i < amount; ++i) {
			if (info_student[i].mark == mark) {
				input_one(info_student, i);
				input = 1;
			}
		}
		if (!input) std::cout << "��������� � ���� ��������� ����� �� �������\n";

		break;

	case 4:
		std::cout << "������� ���������� ������ ����� ��������: ";
		correctInput_num_family(num_family, 0);

		for (int i = 0; i < amount; ++i) {
			if (info_student[i].num_family == num_family) {
				input_one(info_student, i);
				input = 1;
			}
		}
		if (!input) std::cout << "��������� � ���� ��������� ����� �� �������\n";

		break;

	case 5:
		std::cout << "������� ����� ����� ��������: ";
		correctInput_profit(profit_family, 0);

		for (int i = 0; i < amount; ++i) {
			if (info_student[i].profit_family == profit_family) {
				input_one(info_student, i);
				input = 1;
			}
		}
		if (!input) std::cout << "��������� � ���� ��������� ����� �� �������\n";

		break;
	}

	return info_student;
}

void insertionSort(list*& info_student, const int amount) {
	if (info_student[0].name[0] == ' ')
		std::cout << "��� ������� �����, �� ��� �� �������� �����������?\n";

	else {
		for (short i = 0; i < amount; ++i) {
			info_student[i].sort = info_student[i].mark;
			if (info_student[i].profit_family / 4 < 1108) info_student[i].sort = 11;
		}
		for (int i = 1; i < amount; i++)
			for (int j = i; j > 0 && info_student[j].sort > info_student[j - 1].sort; j--) {
				std::swap(info_student[j].profit_family, info_student[j - 1].profit_family);
				std::swap(info_student[j].name, info_student[j - 1].name);
				std::swap(info_student[j].number_group, info_student[j - 1].number_group);
				std::swap(info_student[j].num_family, info_student[j - 1].num_family);
				std::swap(info_student[j].mark, info_student[j - 1].mark);
			}

		std::cout << "���������� ���� ����������� �������\n";
	}
}

void input(list* info_student, const int amount) {
	if (info_student[0].name[0] != ' ') {
		for (int i = 0; i < amount; ++i) {
			std::cout << "\n__" << i + 1 << "__\n"
				<< "�.�.� ��������:  " << info_student[i].name << "\n"
				<< "����� ������ ��������:  " << info_student[i].number_group << "\n"

				<< "C������ ���� ��������:  " << info_student[i].mark << "\n"

				<< "���������� ������ �����:  " << info_student[i].num_family << "\n"

				<< "����� ����� ��������:  " << info_student[i].profit_family << "p" << "\n\n";
		}
	}

	else std::cout << "�� � �����? � ���� ��������� ���� �� ���������\n";
}

void input_file(list* info_student, const int amount, bool& file) {
	if (info_student[0].name[0] == ' ') {
		std::cout << "�� � �����? � ���� ��������� ���� �� ���������\n";
		return;
	}

	std::ofstream fout;
	fout.open("File.txt");

	if (fout.is_open()) {
		for (int i = 0; i < amount; ++i) {
			fout << "\n__" << i + 1 << "__\n"
				<< "�.�.� ��������:  " << info_student[i].name << "\n"
				<< "����� ������ ��������:  " << info_student[i].number_group << "\n"

				<< "C������ ���� ��������:  " << info_student[i].mark << "\n"

				<< "���������� ������ �����:  " << info_student[i].num_family << "\n"

				<< "����� ����� ��������:  " << info_student[i].profit_family << "p" << "\n\n";
		}

		fout.close();

		std::cout << "������ � ���� �������� �������!\n";
	}

	else std::cout << "������ �������� �����!\n";

	std::ofstream read;
	read.open("Read.txt");

	if (read.is_open()) {
		for (int i = 0; i < amount; ++i) {
			read.write((char*)&info_student[i], sizeof(list));
		}

		file = 1;
	}

	else std::cout << "������ �������� �����!\n";
}

void read_file(list* info_student, const int amount, bool& file) {
	if (!file) {
		std::cout << "���� ��� �� ��� ������!\n";
		return;
	}

	std::ifstream in("Read.txt");

	list* new_info_student = new list[amount];
	for (int i = 0; i < amount; ++i)
		in.read((char*)&new_info_student[i], sizeof(list));

	input(new_info_student, amount);

	delete[] new_info_student;
	new_info_student = nullptr;
}