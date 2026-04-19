#include <iostream>
#include <limits>
#include <string>
#include <iomanip>

void unsafe_input() {
    int size = 10;
    char buffer[size];
    std::cout << "Заполните буфер:\n";
    std::cin >> buffer;

    std::cout << "Результат заполения буфера:\n";
    for (int i = 0; i < size; ++i)
    {
        std::cout << buffer[i];
    }

    std::cout << "\n\n";
}

void safe_input()
{
    int size = 10;
    char buffer[size];
    bool valid_input = false;

    while (!valid_input)
    {
        std::cout << "Введите строку (максимум 9 символов): ";

        for (int i = 0; i < size; ++i)
        {
            buffer[i] = '\0';
        }

        std::cin.getline(buffer, size);

        if (std::cin.fail())
        {
            std::cout << "Ошибка! Введено больше 9 символов. Повторите ввод.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            valid_input = true;
        }
    }

    std::cout << "Результат заполнения буфера:\n";
    for (int i = 0; i < size && buffer[i] != '\0'; ++i)
    {
        std::cout << buffer[i];
    }
    std::cout << "\n\n";
}

void safe_input_string()
{
    std::string str;
    std::cout << "Введите строку: ";
    std::getline(std::cin, str); 
    std::cout << "Результат: " << str << "\n\n";
}

int main(){
    while (true)
    {
        int choise;

        std::cout << "\n=== Меню ===\n";
        std::cout << "1. Уязвимый ввод (char[])\n";
        std::cout << "2. Безопасный ввод (char[])\n";
        std::cout << "3. Безопасный ввод (std::string)\n";
        std::cout << "0. Выход\n";
        std::cout << "Ваш выбор: ";

        std::cin >> choise;

        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Ошибка ввода, попробуйте снова\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choise)
        {
        case 1:
            unsafe_input();
            break;
        
        case 2:
            safe_input();
            break;

        case 3:
            safe_input_string();
            break;

        case 0:
            std::cout << "Завершение демонстрации\n";
            return 0;
        
        default:
            std::cout << "Ошибка ввода, попробуйте снова.\n";
            break;
        }
    }
}
