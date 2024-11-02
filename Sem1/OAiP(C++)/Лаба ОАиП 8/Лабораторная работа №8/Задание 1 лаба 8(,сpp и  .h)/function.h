#pragma once
#include "list.h"


struct list* generator_struct(const int amount);
int write_list(list* train, const int amount, int i);
struct list* relloceted(list* train, int& amount);
struct list* redactor(list* train, int amount);
struct list* delete_field(list* train, int& amount);
void input_one(list* train, int i);
struct list* search(list* train, int amount);
void insertionSort(list*& train, const int amount);
void input(list* train, const int amount);
