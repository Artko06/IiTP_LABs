class VigenereCipher:

    @staticmethod
    def encrypt(text: str, key: str, alphabet: str) -> str:
        result = ""
        alphabet_upper = alphabet.upper()
        alphabet_lower = alphabet.lower()
        key_upper = key.upper()
        key_lower = key.lower()
        key_idx = 0

        for char in text:
            if char in alphabet_upper:
                char_idx = alphabet_upper.index(char)
                key_idx_char = alphabet_upper.index(key_upper[key_idx % len(key_upper)])
                new_idx = (char_idx + key_idx_char) % len(alphabet_upper)
                result += alphabet_upper[new_idx]
                key_idx += 1
            elif char in alphabet_lower:
                char_idx = alphabet_lower.index(char)
                key_idx_char = alphabet_lower.index(key_lower[key_idx % len(key_lower)])
                new_idx = (char_idx + key_idx_char) % len(alphabet_lower)
                result += alphabet_lower[new_idx]
                key_idx += 1
            else:
                result += char
        return result

    @staticmethod
    def decrypt(text: str, key: str, alphabet: str) -> str:
        result = ""
        alphabet_upper = alphabet.upper()
        alphabet_lower = alphabet.lower()
        key_upper = key.upper()
        key_lower = key.lower()
        key_idx = 0

        for char in text:
            if char in alphabet_upper:
                char_idx = alphabet_upper.index(char)
                key_idx_char = alphabet_upper.index(key_upper[key_idx % len(key_upper)])
                new_idx = (char_idx - key_idx_char) % len(alphabet_upper)
                result += alphabet_upper[new_idx]
                key_idx += 1
            elif char in alphabet_lower:
                char_idx = alphabet_lower.index(char)
                key_idx_char = alphabet_lower.index(key_lower[key_idx % len(key_lower)])
                new_idx = (char_idx - key_idx_char) % len(alphabet_lower)
                result += alphabet_lower[new_idx]
                key_idx += 1
            else:
                result += char
        return result