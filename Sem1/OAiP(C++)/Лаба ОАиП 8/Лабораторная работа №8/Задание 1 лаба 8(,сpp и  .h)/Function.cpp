#include <iostream>
#include "..//..//Correct_input/Correct_input/Header.h"
#include "function.h"
#include "list.h"


struct list* generator_struct(const int amount) {
	list* train = new list[amount];
	return train;
}

int write_list(list* train, const int amount, int i) {

	for (int copy; i < amount; ++i) {
		printf("%d", i + 1);
		printf(". ��������, �� ������ ������ ����� '-1', ����� ����� �������� �� ����������� ����(��������� � ������� ����)\n");
		printf("������� ����� ����������� ������ ��� 0, ����� ��������� ����: ");
		correctInput_char(train[i].locale_start, i, sizeof(train[i].locale_start));

		if (train[i].locale_start[0] == '0') {
			train[i].locale_start[0] = ' ';
			printf("������ ������ ��� �������� �� ") << printf("%d", i) << printf(" �����\n");
			return i;
		}

		else if (train[i].locale_start[0] == '-' && train[i].locale_start[1] == '1') {
			printf("������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ");
			correctInput_copy(copy, i);

			memcpy(train[i].locale_start, train[copy - 1].locale_start, strlen(train[copy - 1].locale_start));
			train[i].locale_start[strlen(train[copy - 1].locale_start)] = '\0';
		}

		printf("������� ����� �������� ������: ");
		correctInput_char(train[i].locale_end, i, sizeof(train[i].locale_end));

		if (train[i].locale_end[0] == '-' && train[i].locale_end[1] == '1') {
			printf("������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ");
			correctInput_copy(copy, i);

			memcpy(train[i].locale_end, train[copy - 1].locale_end, strlen(train[copy - 1].locale_end));
			train[i].locale_end[strlen(train[copy - 1].locale_end)] = '\0';
		}

		printf("������� ����� ����������� ������(����� enter ���� � ������ ��������������): ");
		correctInput_hour(train[i].hour_start, i);
		if (train[i].hour_start != -1) correctInput_min(train[i].min_start);

		if (train[i].hour_start == -1) {
			printf("������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ");
			correctInput_copy(copy, i);

			train[i].hour_start = train[copy - 1].hour_start;
			train[i].min_start = train[copy - 1].min_start;
		}

		printf("������� ����� �������� ������(����� enter ���� � ������ ��������������): ");
		correctInput_hour(train[i].hour_end, i);

		if (train[i].hour_end != -1) correctInput_min(train[i].min_end);

		if (train[i].hour_end == -1) {
			printf("������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ");
			correctInput_copy(copy, i);

			train[i].hour_end = train[copy - 1].hour_end;
			train[i].min_end = train[copy - 1].min_end;
		}

		printf("������� ���������� ��������� ���� � ������: ");
		correctInput_freelot(train[i].free_lot, i);

		if (train[i].free_lot == -1) {
			printf("������� ����� ����(������� � 1) �� �������� ������ ����������� ������ � �������� � ��� ����: ");
			correctInput_copy(copy, i);

			train[i].free_lot = train[copy - 1].free_lot;
		}
	}
	return i;
}

struct list* relloceted(list* train, int& amount) {
	if (train[amount - 1].locale_start[0] != ' ') {
		int first_amount = amount;

		printf("������� ����� ����� ����� ����������� ������, ������� �� ������: ");
		correctInput_amount(amount, first_amount);
		list* new_train = new list[amount];

		for (short i = 0; i < first_amount; ++i) {
			memcpy(new_train[i].locale_start, train[i].locale_start, strlen(train[i].locale_start));
			new_train[i].locale_start[strlen(train[i].locale_start)] = '\0';
			memcpy(new_train[i].locale_end, train[i].locale_end, strlen(train[i].locale_end));
			new_train[i].locale_end[strlen(train[i].locale_end)] = '\0';

			new_train[i].hour_start = train[i].hour_start;
			new_train[i].hour_end = train[i].hour_end;
			new_train[i].min_start = train[i].min_start;
			new_train[i].min_end = train[i].min_end;
			new_train[i].free_lot = train[i].free_lot;
		}

		delete[] train;
		train = nullptr;

		amount = write_list(new_train, amount, first_amount);
		return new_train;
	}
	else {
		printf("����� ����� ���������, �� ��� ������ �����\n");
		return train;
	}
}

struct list* redactor(list* train, int amount) {
	if (train[0].locale_start[0] == ' ') {
		printf("�� ����� ����������, ��� ���� ������, ��� ������������� ��������?\n");
		return train;
	}

	int num_field, num_struct;
	printf("\n�������� ���� ��� �������������� ��������� ����� ����� � ���������� ����� ��� 0 ��� ������ �� �-���:\n");
	printf("1. ����� ����������� ������\n2. ����� �������� ������\n3. ����� ����������� ������\n4. ����� ��������"
		"������\n5. ���������� ��������� ����\n\n");
	correctInput_redactor_field(num_field);
	if (num_field == 0) return train;

	printf("�������� ����� ��������� ��� ��������������, ������� � 1, ��� 0 ��� ������ �� �-���: ");
	correctInput_redactor_struct(num_struct, amount);
	if (num_struct == 0) return train;

	switch (num_field) {
	case 1:
		printf("%s%d%s", "������� ����� ����������� ������ � ��������� ��� ������� ", num_struct, ": ");
		correctInput_char(train[num_struct - 1].locale_start, 0, sizeof(train[num_struct - 1].locale_start));
		break;

	case 2:
		printf("%s%d%s", "������� ����� �������� ������ � ��������� ��� ������� ", num_struct, ": ");
		correctInput_char(train[num_struct - 1].locale_end, 0, sizeof(train[num_struct - 1].locale_end));
		break;

	case 3:
		printf("%s%d%s", "������� ����� ����������� ������(����� enter ���� � ������ ��������������) "
			"� ��������� ��� ������� ", num_struct, ": ");
		correctInput_hour(train[num_struct - 1].hour_start, 0);
		correctInput_min(train[num_struct - 1].min_start);
		break;

	case 4:
		printf("%s%d%s", "������� ����� �������� ������(����� enter ���� � ������ ��������������) "
			"� ��������� ��� ������� ", num_struct, ": ");
		correctInput_hour(train[num_struct - 1].hour_end, 0);
		correctInput_min(train[num_struct - 1].min_end);
		break;

	case 5:
		printf("%s%d%s", "������� ���������� ��������� ���� � ��������� ��� ������� ", num_struct, ": ");
		correctInput_freelot(train[num_struct - 1].free_lot, 0);
		break;

	}

	return train;
}

struct list* delete_field(list* train, int& amount) {
	if (train[0].locale_start[0] == ' ') {
		printf("�� � ��� �� �������� �������, �������� �� ����?\n");
		return train;
	}

	int num;
	printf("������� ����� ����(������� � 1), ������� ������ �������, ��� 0 ��� ������ �� �-���: ");
	correctInput_delete_field(num, amount);

	if (num == 0) return train;

	amount -= 1;

	list* new_train = new list[amount];

	for (short i = 0, j = 0; j < amount; ++i, ++j) {
		if (i == num - 1) {
			--j;
			continue;
		}

		memcpy(new_train[j].locale_start, train[i].locale_start, strlen(train[i].locale_start));
		new_train[j].locale_start[strlen(train[i].locale_start)] = '\0';
		memcpy(new_train[j].locale_end, train[i].locale_end, strlen(train[i].locale_end));
		new_train[j].locale_end[strlen(train[i].locale_end)] = '\0';

		new_train[j].hour_start = train[i].hour_start;
		new_train[j].hour_end = train[i].hour_end;
		new_train[j].min_start = train[i].min_start;
		new_train[j].min_end = train[i].min_end;
		new_train[j].free_lot = train[i].free_lot;
	}
	delete[] train;
	train = nullptr;
	return new_train;
}

void input_one(list* train, int i) {
	printf("%s%d%s", "\n__", i + 1, "__\n") <<
		printf("%s%s%s", "����� ����������� ������:  ", train[i].locale_start, "\n") <<
		printf("%s%s%s", "����� �������� ������:  ", train[i].locale_end, "\n") <<

		printf("%s%d%s", "����� ����������� ������:  ", train[i].hour_start, ":");
	if (train[i].min_start >= 0 && train[i].min_start <= 9) printf("0");
	printf("%d%s", train[i].min_start, "\n");

	printf("%s%d%s", "����� �������� ������:  ", train[i].hour_end, ":");
	if (train[i].min_end >= 0 && train[i].min_end <= 9) printf("0");
	printf("%d%s", train[i].min_end, "\n");

	printf("%s%d%s", "���������� ��������� ����:  ", train[i].free_lot, "\n\n");
}

struct list* search(list* train, int amount) {
	if (train[0].locale_start[0] == ' ') {
		printf("�� � ����� ��������� �� ����� ��������?\n");
		return train;
	}

	int num_field;
	printf("\n�������� ���� �� ������ �������� ����� ������������� �����, ��������� ����� ����� � ���������� �����,"
		" ��� 0 ��� ������ �� �-���:\n");
	printf("1. ����� ����������� ������\n2. ����� �������� ������\n3. ����� ����������� ������\n4. ����� ��������"
		" ������\n5. ���������� ��������� ����\n\n");
	correctInput_redactor_field(num_field);
	if (num_field == 0) return train;

	char search_char[100];
	bool input = 0;
	int hour, min, free_lot;

	switch (num_field) {
	case 1:
		printf("������� ����� ����������� ������: ");
		correctInput_char(search_char, 0, sizeof(search_char));

		for (int i = 0; i < amount; ++i) {
			if (!strcmp(search_char, train[i].locale_start)) {
				input_one(train, i);
				input = 1;
			}
		}
		if (!input) printf("��������� � ���� ��������� ����� �� �������\n");

		break;

	case 2:
		printf("������� ����� �������� ������: ");
		correctInput_char(search_char, 0, sizeof(search_char));

		for (int i = 0; i < amount; ++i) {
			if (!strcmp(search_char, train[i].locale_end)) {
				input_one(train, i);
				input = 1;
			}
		}
		if (!input) printf("��������� � ���� ��������� ����� �� �������\n");

		break;

	case 3:
		printf("������� ����� ����������� ������(����� enter ���� � ������ ��������������): ");
		correctInput_hour(hour, 0);
		correctInput_min(min);

		for (int i = 0; i < amount; ++i) {
			if (train[i].hour_start == hour && train[i].min_start == min) {
				input_one(train, i);
				input = 1;
			}
		}
		if (!input) printf("��������� � ���� ��������� ����� �� �������\n");

		break;

	case 4:
		printf("������� ����� �������� ������(����� enter ���� � ������ ��������������): ");
		correctInput_hour(hour, 0);
		correctInput_min(min);

		for (int i = 0; i < amount; ++i) {
			if (train[i].hour_end == hour && train[i].min_end == min) {
				input_one(train, i);
				input = 1;
			}
		}
		if (!input) printf("��������� � ���� ��������� ����� �� �������\n");

		break;

	case 5:
		printf("������� ���-�� ��������� ���� � ������: ");
		correctInput_freelot(free_lot, 0);

		for (int i = 0; i < amount; ++i) {
			if (train[i].free_lot == free_lot) {
				input_one(train, i);
				input = 1;
			}
		}
		if (!input) printf("��������� � ���� ��������� ����� �� �������\n");

		break;
	}

	return train;
}

void insertionSort(list*& train, const int amount) {
	if (train[0].locale_start[0] == ' ')
		printf("��� ������� �����, �� ��� �� �������� �����������?\n");

	else {
		for (int i = 1; i < amount; i++)
			for (int j = i; j > 0 && train[j].free_lot > train[j - 1].free_lot; j--) {
				std::swap(train[j].free_lot, train[j - 1].free_lot);
				std::swap(train[j].locale_start, train[j - 1].locale_start);
				std::swap(train[j].locale_end, train[j - 1].locale_end);
				std::swap(train[j].hour_start, train[j - 1].hour_start);
				std::swap(train[j].hour_end, train[j - 1].hour_end);
				std::swap(train[j].min_start, train[j - 1].min_start);
				std::swap(train[j].min_end, train[j - 1].min_end);
			}

		printf("���������� ���� ����������� �������\n");
	}
}

void input(list* train, const int amount) {
	if (train[0].locale_start[0] != ' ') {
		for (int i = 0; i < amount; ++i) {
			if (train[i].locale_start[0] != '0') {
				printf("%s%d%s", "\n__", i + 1, "__\n") <<
					printf("%s%s%s", "����� ����������� ������:  ", train[i].locale_start, "\n") <<
					printf("%s%s%s", "����� �������� ������:  ", train[i].locale_end, "\n") <<

					printf("%s%d%s", "����� ����������� ������:  ", train[i].hour_start, ":");
				if (train[i].min_start >= 0 && train[i].min_start <= 9) printf("0");
				printf("%d%s", train[i].min_start, "\n");

				printf("%s%d%s", "����� �������� ������:  ", train[i].hour_end, ":");
				if (train[i].min_end >= 0 && train[i].min_end <= 9) printf("0");
				printf("%d%s", train[i].min_end, "\n");

				printf("%s%d%s", "���������� ��������� ����:  ", train[i].free_lot, "\n\n");
			}
		}
	}
	else printf("�� � �����? � ���� ��������� ���� �� ���������\n");
}
