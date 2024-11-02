#include <iostream>
#include "..//..//..//Correct_Input/Correct_Input/Header.h"

void divisibility(long long num, short what_sistem) {
    long long check_num = what_sistem;

    while ((num > (check_num << 1)) && ((check_num << 1) > 0))
        check_num <<= 1;

    while ((check_num + 500000) <= num)
        check_num += 500000;

    while ((check_num + what_sistem) <= num)
        check_num += what_sistem;


    (num == check_num) ? std::cout << "Число делиться нацело на " << what_sistem << '\n' :
        std::cout << "Число НЕ делиться нацело на " << what_sistem << '\n';

}


int main() {
    setlocale(LC_ALL, "RU");
    short replay;
    std::cout << "Введите кол-во чисел которые будем проверять на делимость: ";
    correctInput_S_N(replay);

    for (short i = 0; i < replay; i++) {
        long long num;
        short what_sistem;

        std::cout << "Введите число для проверки делимоти: ";
        correctInput_LL(num);
        if (num < 0) { num = ~num + 1; }

        std::cout << "Проверим делимость числа на...(введите число) ";
        correctInput_S_N(what_sistem);

        divisibility(num, what_sistem);
    }
	return 0;
}