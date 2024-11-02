#pragma once

class list {

private:
	char name[1000] = " ";
	int num, day, month, year;
	union {
		float cost;
	};

public:
	void setDay(int day) {
		this->day = day;
	}
	int getDay(){
		return this->day;
	}

	void setMonth(int month) {
		this->month = month;
	}
	int getMonth() {
		return this->month;
	}

	void setYear(int year) {
		this->year = year;
	}
	int getYear() {
		return this->year;
	}

	void setNum(int num) {
		this->num = num;
	}
	int getNum() {
		return this->num;
	}

	void setCost(float cost) {
		this->cost = cost;
	}
	float getCost() {
		return this->cost;
	}

	void setName(const char* name) {
		strcpy_s(this->name, name);
	}
	char* getName() {
		return this->name;
	}

	void setFirstName(char name) {
		this->name[0] = name;
	}
	char getFirstName() {
		return this->name[0];
	}

	char getCheckMinys() {
		return this->name[0];
	}

	char getCheckOne() {
		return this->name[1];
	}

	void setXname(int X, char X_char) {
		this->name[X] = X_char;
	}
};