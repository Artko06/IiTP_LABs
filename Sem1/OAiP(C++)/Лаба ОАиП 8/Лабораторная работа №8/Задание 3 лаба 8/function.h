#pragma once
#include "list.h"


list* generator_struct(const int amount);
int write_list(list* info_storage, const int amount, int i);
list* relloceted(list* info_storage, int& amount);
list* redactor(list* info_storage, int amount, bool& file);
list* delete_field(list* info_storage, int& amount);
void input_one(list* info_storage, int i);
list* search(list* info_storage, int amount);
void insertionSort(list*& info_storage, const int amount);
void input(list* info_storage, const int amount);
void input_file(list* info_storage, const int amount, bool& file);
void read_file(list* info_storage, const int amount, bool file);
