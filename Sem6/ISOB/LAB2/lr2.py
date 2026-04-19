import os
from caesar_cipher import CaesarCipher
from vigenere_cipher import VigenereCipher

"""
Реализовать программное средство шифрования и дешифрования
текстовых файлов при помощи шрифта Цезаря (шифр сдвига, кода цезаря) и шифра Виженера

Implement a software tool for encrypting and decrypting
text files using the Caesar font (shift cipher, Caesar code) and the Vigenere cipher
"""

class FileCipherApp:
    def __init__(self):
        self.caesar = CaesarCipher()
        self.vigenere = VigenereCipher()
        self.english_alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        self.russian_alphabet = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"

    def get_alphabet(self, language: str) -> str:
        alphabets = {
            'en' : self.english_alphabet,
            'ru' : self.russian_alphabet,
        }

        language_lowercase = language.lower()

        if language_lowercase not in alphabets:
            print("Was select default language: English")
            return self.english_alphabet
        else:
            return alphabets[language_lowercase]


def main():
    if not os.path.exists("input.txt"):
        print("Файл input.txt не найден!")
        return

    with open('input.txt', 'r', encoding='utf-8') as f:
        text = f.read()

    file_cipher = FileCipherApp()
    selected_alphabet = file_cipher.get_alphabet('en')

    while True:
        choice_lang = input("Выберите языке (en/ru): ")

        if choice_lang not in ['en', 'ru']:
            print("Ошибка ввода, попробуйте снова")
            continue
        else:
            selected_alphabet = file_cipher.get_alphabet(choice_lang)
            break

    print("Выберите операцию:")
    print("1. Зашифровать (Шифр Цезаря)")
    print("2. Расшифровать (Шифр Цезаря)")
    print("3. Зашифровать (Шифр Виженера)")
    print("4. Расшифровать (Шифр Виженера)")

    result = "Error output!"

    while True:
        choice = input("Ваш выбор (1/2/3/4): ")

        if choice not in ['1', '2', '3', '4']:
            print("Ошибка ввода, попробуйте снова")
            continue
        else:
            if choice in ['1', '2']:
                while True:
                    shift = input("Введите сдвиг (целое число): ")
                    try:
                        shift = int(shift)
                        break
                    except ValueError:
                        print("Сдвиг должен быть целым числом! Попробуйте снова.")

                if choice == '1':
                    result = file_cipher.caesar.encrypt(text, shift, selected_alphabet)
                elif choice == '2':
                    result = file_cipher.caesar.decrypt(text, shift, selected_alphabet)
            elif choice in ['3', '4']:
                while True:
                    key = input("Введите ключ: ").strip()

                    if not key:
                        print("Ключ не может быть пустым! Попробуйте снова.")
                        continue

                    valid_key = True
                    for char in key.upper():
                        if char not in selected_alphabet.upper():
                            print("Ключ должен состоять из букв выбранного алфавита")
                            valid_key = False
                            break

                    if not valid_key:
                        continue

                    break

                if choice == '3':
                    result = file_cipher.vigenere.encrypt(text, key, selected_alphabet)
                elif choice == '4':
                    result = file_cipher.vigenere.decrypt(text, key, selected_alphabet)

            break


    with open("output.txt", 'w', encoding='utf-8') as f:
        f.write(result)

    print("\nГотово! Результат сохранен в 'output.txt'")

    preview = result[:250] + "..." if len(result) > 250 else result
    print(f"\nПредпросмотр результата:\n{preview}")


if __name__ == "__main__":
    main()