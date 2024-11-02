#pragma once
#include "list.h"


struct list* generator_struct(const int amount);
int write_list(list* info_student, const int amount, int i);
struct list* relloceted(list* info_student, int& amount);
struct list* redactor(list* info_student, int amount);
struct list* delete_field(list* info_student, int& amount);
void input_one(list* info_student, int i);
struct list* search(list* info_student, int amount);
void insertionSort(list*& info_student, const int amount);
void input(list* info_student, const int amount);
void input_file(list* info_student, const int amount, bool& file);
void read_file(list* info_student, const int amount, bool& file);