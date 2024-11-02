#include <iostream>

int main() {
    setlocale(LC_ALL, "RU");
    char* input = NULL;
    short length = 0;
    int num_char;

    std::cout << "Введите символы: ";

    input = (char*)realloc(input, 2);
    while ((num_char = getchar()) != '\n') {
        input[length] = num_char;
        length++;
        input = (char*)realloc(input, 2 + length);
    }

    input[length] = '\0'; // Добавляем нулевой символ для завершения строки

    std::cout << "Различные символы в строке:\n";

    short iter_while = 0;

    for (int i_char = 0; i_char < 256; i_char++) { // Можно тоже в ф-цию void
       while(iter_while < length) {
           if (input[iter_while] == (char)i_char) {
               std::cout << (char)i_char << ' ';
               break;
           }
           iter_while++;
       }
       iter_while = 0;
    }

    free(input);
    input = NULL;
    return 0;
}