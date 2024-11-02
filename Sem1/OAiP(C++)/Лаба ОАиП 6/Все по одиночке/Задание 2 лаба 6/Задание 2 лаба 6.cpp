#include <iostream>


char** arr_generator(char* words, const short space, const short length, const int max) {
    
    char** one_word = new char*[space];

    for (int i = 0; i < space; i++) {
        one_word[i] = new char[max + 1];
    }
    return one_word;
}

int main() {
    setlocale(LC_ALL, "RU");
    char* words = NULL;
    short length = 0;
    int num_char;
    short space = 1;
    char** one_word = NULL;

    std::cout << "Введите символы: ";

    words = (char*)realloc(words, 2);
    while ((num_char = getchar()) != '\n') {
        words[length] = num_char;
        length++;
        words = (char*)realloc(words, 2 + length);
    }
    words[length] = '\0'; // Добавляем нулевой символ для завершения строки

    for (int i = 0; i < length; i++) {
        if (isspace(words[i]) && i != 0) space++;
    }

    int* arr = new int[space];
    for (int i = 0, i_arr = 0; i < length; i++) {
        if (words[i + 1] == ' ' && !i_arr) {
            arr[i_arr] = i + 1;
            i_arr++;
        }
        else if (words[i + 1] == ' ' || i == length - 1) {
            arr[i_arr] = i + 1 - i_arr - arr[i_arr - 1];
            i_arr++;
        }
    }

    int max = arr[0];

    for (int i = 1; i < space; i++) {
        if (max < arr[i]) { max = arr[i]; }
    }

    one_word = arr_generator(words, space, length, max);

    char* last_letter = new char[space + 1];

    for (int i = 0, el = 0, num_word = 0, last = 0; i < length; i++, el++) {
        one_word[num_word][el] = words[i];
        if (words[i + 1] == ' ' || i == length - 1) {
            last_letter[last] = words[i];
            last++;
            if (last == space) { last_letter[last] = '\0'; }
            one_word[num_word][el+1] = '\0';
            num_word++;
            i++;
            el = -1;
            continue;
        }
    }

    for (int num_word = 0; num_word < space; num_word++) {
        for (int i = 0; one_word[num_word][i] >= 65 && one_word[num_word][i] <= 122; i++) {
            if (one_word[num_word][0] == last_letter[num_word]) { 
                std::cout << one_word[num_word][i];
                if (one_word[num_word][i + 1] == '\0' || one_word[num_word][i] < 65 || one_word[num_word][i] > 122) { std::cout << ' '; }
            }
            if (i + 1 == max) { break; }
        }
    }

    free(words);
    words = NULL;
    for (int i = 0; i < space; i++) {
        free(one_word[i]);
        one_word[i] = NULL;
    }
    free(one_word);
    one_word = NULL;

    return 0;
}