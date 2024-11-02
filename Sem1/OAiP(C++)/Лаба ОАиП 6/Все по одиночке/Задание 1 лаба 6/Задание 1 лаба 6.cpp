#include <iostream>

int main() {
    setlocale(LC_ALL, "RU");
    char* input = NULL;
    short length = 0;
    int num_char;
    short space = 1;

    std::cout << "Введите символы: ";

    input = (char*)realloc(input, 2);
    while (( num_char = getchar()) != '\n') {
        input[length] = num_char;
        length++;
        input = (char*)realloc(input, 2 + length);
    }
    input[length] = '\0'; // Добавляем нулевой символ для завершения строки

    for (int i = 0; i < length; i++) { 
        if (isspace(input[i]) && i != 0 ) space++;
    }

    short* arr_space = NULL;
    arr_space = (short*)calloc(space, sizeof(short));
    short iter_while = 0;

    for (int i = 0; i < space;) {
        while (iter_while < length){
            if (isalpha(input[iter_while])) arr_space[i]++;
            else { break; }
            iter_while++;
        }
        i++;
        iter_while++;
    }

    short max_string = arr_space[0];
    short number_string = 1;

    for (int i = 1; i < space; i++) { // В функцию поиск max
        if (max_string < arr_space[i]) {
            max_string = arr_space[i];
            number_string = i + 1;
        }
    }

    std::cout << "Слова с наибольшим кол-вом букв в строке под номером: " << number_string << '\n' << "Кол-во букв: " << max_string << '\n';

    free(arr_space);
    arr_space = NULL;
    free(input);
    input = NULL;
    return 0;
}