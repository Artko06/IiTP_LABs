#include <iostream>
#include <string>
#include "..//..//..//Correct_Input/Correct_Input/Header.h"


short correctInput_sistem(short& num) {
    while (true) {
        if (!(std::cin >> num)) {
            std::cin.clear();
            while (std::cin.get() != '\n');
            std::cout << "Неверный ввод \n";
            continue;
        }
        if (std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод \n";
            continue;
        }
        if (num < 2 || num > 36) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Неверный ввод \n";
            continue;
        }
        break;
    }
    return num;
}

short* translate(const short sistem, long long num, std::string &num_str, short* &num_short) {
    short i = 0, ost;
    std::string for_string;

    for (; num > 0; i++) {
        ost = num % sistem;
        if (ost > 9) for_string = (char)(num % sistem + 55);
        else for_string = (char)(num % sistem + 48);
        num_short[i] = ost;

        num_str.insert(0, for_string);
        num /= sistem;
    }

    std::cout << num_str << '\n';
    return num_short;
}

std::string sum(short* &num1_short, short* &num2_short, short sistem, bool minys1, bool minys2) {
    std::string sum;
    short dop = 0;
    std::string convert_to_str;
    
    if (minys1 != minys2) {
        for (short i = 0; i < 70; i++) {
            if (minys2 == 1) {
                if (num1_short[i] >= 0) {
                    if (num2_short[i] < 0) num2_short[i] = 0;
                    num2_short[i] = sistem - 1 - num2_short[i];
                }
            }
        }
            for (short i = 0; i < 70; i++) {
                if (num2_short[i] < sistem - 1 && num2_short[i] >= 0) {
                    num2_short[i]++;
                    break;
                }
                else if (num2_short[i] == sistem - 1) num2_short[i] = 0;
                else num2_short[i] = 1;
            }
    }
  
    for (short i = 0; i < 70; i++) {
        if (num1_short[i] < 0 && num2_short[i] < 0 && !dop) {
            if (minys1 != minys2) sum.erase(0, 1);
            if(sum.at(0) == '0' && sum.size() > 1) sum.erase(0, 1);
            return sum;
        }
            if (num1_short[i] < 0) num1_short[i] = 0;
            if (num2_short[i] < 0) num2_short[i] = 0;
            if ((num1_short[i] + num2_short[i] + dop) > sistem - 1) {
                if (num1_short[i] + num2_short[i] + dop - sistem > 9) {
                    convert_to_str = (char)((num1_short[i] + num2_short[i] + dop - sistem + 55));
                    sum.insert(0, convert_to_str);
                }
                else sum.insert(0, std::to_string(num1_short[i] + num2_short[i] + dop - sistem));
                if (!dop) dop++;
            }

            else {
                if (num1_short[i] + num2_short[i] + dop > 9) {
                    convert_to_str = (char)((num1_short[i] + num2_short[i] + dop + 55));
                    sum.insert(0, convert_to_str);
                }
                else sum.insert(0, std::to_string(num1_short[i] + num2_short[i] + dop));
                if (dop == 1) dop--;
            }
    }
    
}


int main() {
    setlocale(LC_ALL, "RU");

    char replay = '+';
    while (replay == '+') {
    bool minys1 = 0, minys2 = 0;
    char simbol;
    long long num1, num2;
    short sistem;
    short* num1_short = new short[70];
    short* num2_short = new short[70];
    std::string num_str1, num_str2;

    std::cout << "Введите основание системы исчисления от 2 до 36 включитено: ";
    correctInput_sistem(sistem);
    std::cout << "Введите '+' или '-' в зависимости от математической операции, которую хотите произвести: ";
    std::cin >> simbol;
    std::cout << "Введите первое и второе число соотвественно: ";
    correctInput_LL(num1);
    correctInput_LL(num2);

    if (simbol == '-') num2 = ~num2 + 1;

    if (num1 < 0) {
        num1 = ~num1 + 1;
        minys1 = 1;
    }

    if (num2 < 0) {
        num2 = ~num2 + 1;
        minys2 = 1;
    }

    if (num2 > num1) {
        num1 = num1 ^ num2;
        num2 = num1 ^ num2;
        num1 = num1 ^ num2;

        minys1 = minys1 ^ minys2;
        minys2 = minys1 ^ minys2;
        minys1 = minys1 ^ minys2;
    }

    std::cout << "Эти числа в " << sistem << "-ичой системе исчисления(по модулю):\n";
    translate(sistem, num1, num_str1, num1_short);
    translate(sistem, num2, num_str2, num2_short);

    std::cout << "Сумма этих чисел в заданной системе исчисления:\n";
    if (minys1) std::cout << '-';
    std::cout << sum(num1_short, num2_short, sistem, minys1, minys2);

    std::cout << "\nВведите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    std::cin >> replay;
    }

    return 0;
}