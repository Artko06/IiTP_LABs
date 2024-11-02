#include <iostream>
#include <fstream>
#include "..//..//Correct_input3/Correct_input3/Header.h"
#include "function.h"
#include "list.h"


int main() {
	setlocale(LC_ALL, "RU");
	int amount = 0, choise;
	bool file = 0;

	std::cout << "Список товаров, имеющихся на складе, включает в себя наименование товара, количество единиц"
		" товара, цену единицы и дату поступления товара на склад. Вывести список товаров, стоимость которых"
		" превышает 100.341 рублей\n"
		"Введите сколько товаров хотите описать или 0 для завершения программы: ";
	amount = correctInput_amount(amount);

	if (amount == 0) return 0;

	list* info_storage;
	info_storage = generator_struct(amount);

	std::cout << "\nВыберите ф-цию в зависимости от её номера или 0 для завершения программы:\n"
		"1. Заполнение созданного массива структур(перезаполнение)"
		"\n2. Функция расширения массива структур и заполнение\n3. Ф-ция удаления поля структуры\n"
		"4. Ф-ция изменения элемента структуры(без полной перезаписи bin файла)\n"
		"5. Отсортировать массив структур по заданым условиям\n6. Вывод массива структур в консоль\n"
		"7. Вывод на экран структур с заданным значением элемента\n"
		"8. Запись результатов работы программы в файл\n9. Прочитать данные из .bin файла\n";

	while (correctInput_choise(choise)) {
		if (choise == 0) break;

		switch (choise) {
		case 1:
			amount = write_list(info_storage, amount, 0);
			break;

		case 2:
			info_storage = relloceted(info_storage, amount);
			break;

		case 3:
			info_storage = delete_field(info_storage, amount);
			break;

		case 4:
			info_storage = redactor(info_storage, amount, file);
			break;

		case 5:
			insertionSort(info_storage, amount);
			break;

		case 6:
			input(info_storage, amount);
			break;

		case 7:
			info_storage = search(info_storage, amount);
			break;

		case 8:
			input_file(info_storage, amount, file);
			break;

		case 9:
			read_file(info_storage, amount, file);
			break;
		}
		std::cout << "\nВыберите ф-цию в зависимости от её номера или 0 для завершения программы:\n"
			"1. Заполнение созданного массива структур(перезаполнение)"
			"\n2. Функция расширения массива структур и заполнение\n3. Ф-ция удаления поля структуры\n"
			"4. Ф-ция изменения элемента структуры(без полной перезаписи bin файла)\n"
			"5. Отсортировать массив структур по заданым условиям\n6. Вывод массива структур в консоль\n"
			"7. Вывод на экран структур с заданным значением элемента\n"
			"8. Запись результатов работы программы в файл\n9. Прочитать данные из .bin файла\n";
	}
	
	return 0;
}