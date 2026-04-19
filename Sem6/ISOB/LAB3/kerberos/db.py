from cryptography.fernet import Fernet

TGS_SECRET_KEY = Fernet.generate_key()

USER_DB = {
    "artem_kokhan": "artem_kokhan_password_123",
}

SERVICE_DB = {
    "file_storage": "storage_server_secret_key",
}