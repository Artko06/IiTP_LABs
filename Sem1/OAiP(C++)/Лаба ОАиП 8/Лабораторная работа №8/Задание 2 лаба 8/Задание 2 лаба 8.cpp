#include <iostream>
#include <fstream>
#include "..//..//Correct_input2/Correct_input2/Header.h"
#include "function.h"
#include "list.h"


int main() {
	setlocale(LC_ALL, "RU");
	int amount, choise;
	bool file = 0;

	std::cout << "Для получения места в общежитии формируется список студентов, который включает Ф.И.О. студента, "
		"группу, средний балл, доход на члена семьи. Общежитие в первую очередь предоставляется тем, у кого доход"
		" семьи меньше двух минимальных зарплат(2 * 554р = 1108р), затем остальным в порядке уменьшения "
		"среднего балла.\nВведите количество студентов для заполнения или 0 для завершения программы: ";
	correctInput_amount(amount);

	if (amount == 0) return 0;

	list* info_student;
	info_student = generator_struct(amount);

	std::cout << "\nВыберите ф-цию в зависимости от её номера или 0 для завершения программы:\n"
		"1. Заполнение созданного массива структур(перезаполнение)"
		"\n2. Функция расширения массива структур и заполнение\n3. Ф-ция удаления поля структуры\n"
		"4. Ф-ция изменения элемента структуры\n"
		"5. Отсортировать массив структур по заданым условиям\n6. Вывод массива структур в консоль\n"
		"7. Вывод на экран структур с заданным значением элемента\n"
		"8. Запись результатов работы программы в файл\n9. Вывести данные из .txt файла\n";
	while (correctInput_choise(choise)) {
		if (choise == 0) break;

		switch (choise) {
		case 1:
			amount = write_list(info_student, amount, 0);
			break;

		case 2:
			info_student = relloceted(info_student, amount);
			break;

		case 3:
			info_student = delete_field(info_student, amount);
			break;

		case 4:
			info_student = redactor(info_student, amount);
			break;

		case 5:
			insertionSort(info_student, amount);
			break;

		case 6:
			input(info_student, amount);
			break;

		case 7:
			info_student = search(info_student, amount);
			break;

		case 8:
			input_file(info_student, amount, file);
			break;

		case 9:
			read_file(info_student, amount, file);
			break;
		}
		std::cout << "\nВыберите ф-цию в зависимости от её номера или 0 для завершения программы:\n"
			"1. Заполнение созданного массива структур(перезаполнение)"
			"\n2. Функция расширения массива структур и заполнение\n3. Ф-ция удаления поля структуры\n"
			"4. Ф-ция изменения элемента структуры\n"
			"5. Отсортировать массив структур по заданым условиям\n6. Вывод массива структур в консоль\n"
			"7. Вывод на экран структур с заданным значением элемента\n"
			"8. Запись результатов работы программы в файл\n9. Вывести данные из .txt файла\n";
	}
	return 0;
}