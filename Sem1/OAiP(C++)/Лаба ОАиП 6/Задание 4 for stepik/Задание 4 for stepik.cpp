#include <iostream>
#include <string>


int main() {
    std::string str;
    std::getline(std::cin, str);

    for (int i = 0; i < str.length(); ++i) {
        if (str.at(i) == 'Q' || str.at(i) == 'W' || str.at(i) == 'X' || str.at(i) == 'C') { //Q W X C
            str.erase(i, 1);
            --i;
        }
    }

    for (int i = 0; i < str.length(); ++i) {
        if (i + 1 < str.length() && (str.at(i) == 'p' || str.at(i) == 'P') &&
            (str.at(i + 1) == 'h' || str.at(i + 1) == 'H')) { //ph, Ph
            str.at(i + 1) = 'f';
            if (str.at(i) == 'P') str.at(i + 1) -= 32;
            str.erase(i, 1);
        }

        else if (i + 1 < str.length() && (str.at(i) == 't' || str.at(i) == 'T') &&
            (str.at(i + 1) == 'h' || str.at(i + 1) == 'H')) { //th, Th
            str.at(i + 1) = 'z';
            if (str.at(i) == 'T') str.at(i + 1) -= 32;
            str.erase(i, 1);
        }

        else if (i + 1 < str.length() && (str.at(i) == 'e' || str.at(i) == 'E') && // ee, Ee
            (str.at(i + 1) == 'e' || str.at(i + 1) == 'E')) {
            str.at(i + 1) = 'i';
            if (str.at(i) == 'E') str.at(i + 1) -= 32;
            str.erase(i, 1);
        }

        else if (i + 1 < str.length() && str.at(i) == 'q' && (str.at(i + 1) == 'u' || //qu
            str.at(i + 1) == 'U')) {
            str.at(i) = 'k';
            str.at(i + 1) = 'v';
        }

        else if (i + 2 < str.length() && (str.at(i) == 'y' || str.at(i) == 'Y') //you, You
            && (str.at(i + 1) == 'o' || str.at(i + 1) == 'O')
            && (str.at(i + 2) == 'u' || str.at(i + 2) == 'U')) {
            str.at(i + 2) = 'u';
            if (str.at(i) == 'Y') str.at(i + 2) -= 32;
            str.erase(i, 2);
        }

        else if (i + 1 < str.length() && (str.at(i) == 'o' || str.at(i) == 'O') && //oo, Oo
            (str.at(i + 1) == 'o' || str.at(i + 1) == 'O')) {
            str.at(i + 1) = 'u';
            if (str.at(i) == 'O') str.at(i + 1) -= 32;
            str.erase(i, 1);
        }

        else if (str.at(i) == 'q') str.at(i) = 'k'; // q

        else if (str.at(i) == 'x') { // x
            str.at(i) = 'k';
            str.insert(i + 1, "s");
        }

        else if (str.at(i) == 'w') str.at(i) = 'v'; // w
    }

     for (int i = 1; i < str.length(); ++i) { // ce, ci, cy, etc...
        if (str.at(i - 1) == 'c' && (str.at(i) == 'e' || str.at(i) == 'i' ||
            str.at(i) == 'y' || str.at(i) == 'E' || str.at(i) == 'I' || str.at(i) == 'Y')) {
            str.at(i - 1) = 's';
        }

        else if (str.at(i - 1) == 'c') str.at(i - 1) = 'k';
     }

    for (int i = 1; i < str.length(); ++i) { // двойные согласные
        if ((str.at(i - 1) >= 65 && str.at(i - 1) <= 90) || (str.at(i - 1) >= 97 &&
            str.at(i - 1) <= 122)) {

            if (str.at(i - 1) == str.at(i) || str.at(i - 1) == str.at(i) - 32 ||
                str.at(i - 1) - 32 == str.at(i)) {

                if (str.at(i) != 'a' && str.at(i) != 'o' && str.at(i) != 'u' && str.at(i) != 'e' &&
                    str.at(i) != 'i' && str.at(i) != 'y' && str.at(i) != 'A' && str.at(i) != 'O' &&
                    str.at(i) != 'U' && str.at(i) != 'E' && str.at(i) != 'I' && str.at(i) != 'Y') {

                    str.erase(i, 1);
                    --i;

                }
            }
        }
    }
    std::cout << str;
    return 0;
}