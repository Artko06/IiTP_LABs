class CaesarCipher:

    @staticmethod
    def encrypt(text: str, shift: int, alphabet: str) -> str:
        result = ""
        alphabet_upper = alphabet.upper()
        alphabet_lower = alphabet.lower()

        for char in text:
            if char in alphabet_upper:
                idx = (alphabet_upper.index(char) + shift) % len(alphabet_upper)
                result += alphabet_upper[idx]
            elif char in alphabet_lower:
                idx = (alphabet_lower.index(char) + shift) % len(alphabet_lower)
                result += alphabet_lower[idx]
            else:
                result += char
        return result


    @staticmethod
    def decrypt(text: str, shift: int, alphabet: str) -> str:
        return CaesarCipher.encrypt(text, -shift, alphabet)