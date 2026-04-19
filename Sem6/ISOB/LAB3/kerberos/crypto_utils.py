import base64
import hashlib
from cryptography.fernet import Fernet

def get_fernet_key(secret: str):
    """Создает ключ Fernet на основе строки (пароля)"""
    key = hashlib.sha256(secret.encode()).digest()
    return Fernet(base64.urlsafe_b64encode(key))

def encrypt(key_source, data: str):
    """Шифрует строку. key_source может быть строкой или уже объектом Fernet"""
    f = get_fernet_key(key_source) if isinstance(key_source, str) else Fernet(key_source)
    return f.encrypt(data.encode())

def decrypt(key_source, token):
    """Расшифровывает данные в строку"""
    f = get_fernet_key(key_source) if isinstance(key_source, str) else Fernet(key_source)
    return f.decrypt(token).decode()