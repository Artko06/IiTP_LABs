#include <iostream>
#include <string>
#include "..//..//..//Correct_Input/Correct_Input/Header.h"


short max_bit(long long num) {
    short max_bit = 0;

    while (num > 0) {
        num >>= 1;
        max_bit++;
    }

    return max_bit;
}

void num_to_bin(long long num, std::string &bin, const short max_bit, bool minys) {
    while(num > 0) {
        if (num & 1) bin.insert(0, "1");
        else bin.insert(0, "0"); 

        num >>= 1;
    }

    if (minys) {
        bin.insert(0, "1");

        for (short i = 1; i < bin.length(); i++) { // перевод из прямого кода в обратку
            if (bin.at(i) == '0') bin.at(i) = '1';
            else if (bin.at(i) == '1') bin.at(i) = '0';
        }
        for (short i = bin.length() - 1; i > 0; i--) { // перевод из обратки в доп код
            if (bin.at(i) == '1') bin.at(i) = '0';
            else if (bin.at(i) == '0') {
                bin.at(i) = '1';
                break;
            }
        }
    }
    else bin.insert(0, "0"); // знаковый bit

    std::cout << bin << '\n';
}

void sum(std::string bin1, std::string bin2, bool minys1, bool minys2, bool dop_null, bool sum_minys) {
    std::string sum;
    short dop = 0;

        for (short i = bin1.length() - 1, j = bin2.length() - 1; i > 0; i--, j--) {
            if (j == - 1) j++;

            if ((bin1.at(i) + bin2.at(j) + dop) - 96 > 1) {
                if ((bin1.at(i) + bin2.at(j) + dop) - 96 == 2) {
                    sum.insert(0, std::to_string(0));
                    if (!dop) dop++;
                }
                else sum.insert(0, std::to_string(1));
            }

            else if ((bin1.at(i) + bin2.at(j) + dop) - 96 == 1) {
                if (dop == 1) dop--;
                sum.insert(0, std::to_string(1));
            }

            else sum.insert(0, std::to_string(0));
        }

        if (dop && ((!minys1 && !minys2) || (minys1 && minys2))) sum.insert(0, std::to_string(1));//?
    
    if(minys1 && minys2) sum.insert(0, std::to_string(1));

    else if(sum_minys && !(minys1 && minys2)) sum.insert(0, std::to_string(1));

    else sum.insert(0, std::to_string(0));

    if(dop_null) sum.insert(1, std::to_string(0));

    std::cout << "Сумма в дополнительном коде: " << sum << "\n\n";

    if (sum.at(0) == '1') { // Отрицательное :(

        for (int i = sum.length() - 1; i > 0; i--) { // перевод из доп в обратный код
            if (sum.at(i) == '0') sum.at(i) = '1';
            else if (sum.at(i) == '1') {
                sum.at(i) = '0';
                break;
            }
        }

        for (int i = 1; i < sum.length(); i++) { // перевод из обратки в прямой код
            if (sum.at(i) == '0') sum.at(i) = '1';
            else if (sum.at(i) == '1') sum.at(i) = '0';
        }
    }

    long long sum10 = 0;
    for (short i = sum.length() - 1, exp = 0; i > 0 ; i--, exp++)
        if(sum.at(i) != 48) sum10 += pow(2, exp);

    if (sum.at(0) == 49) sum10 = ~sum10 + 1;
    std::cout << "Cумма в десятичке: " << sum10 << '\n';
}


int main() {
    setlocale(LC_ALL, "RU");
    char replay = '+';
    while (replay == '+') {

    bool minys1 = 0, minys2 = 0, dop_null = 0, sum_minys = 0;
    long long num1, num2;
    std::string bin1, bin2;

    std::cout << "Введите первое и второе число в десятичной системе исчисления(в естественной форме): ";
    correctInput_LL(num1);
    correctInput_LL(num2);

    if (num1 + num2 < 0) sum_minys = 1;

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

    if (max_bit(num1 + num2 - 1) > max_bit(num1)) dop_null = 1;

    std::cout << "Эти числа в дополнительном коде:\n";
    num_to_bin(num1, bin1, max_bit(num1), minys1);
    num_to_bin(num2, bin2, max_bit(num2), minys2);

    sum(bin1, bin2, minys1, minys2, dop_null, sum_minys);

    std::cout << "\nВведите '+', если хотите повторить или любой другой символ, чтобы завершить программу: ";
    std::cin >> replay;
    }

    return 0;
}