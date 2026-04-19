from kerberos import crypto_utils
from kerberos.db import USER_DB, TGS_SECRET_KEY
from cryptography.fernet import Fernet


def get_tgt(client_id):
    if client_id not in USER_DB:
        raise Exception("Пользователь не найден")

    session_key_tgs = Fernet.generate_key()

    tgt_data = f"{client_id}|{session_key_tgs.decode()}"
    tgt = crypto_utils.encrypt(TGS_SECRET_KEY, tgt_data)

    client_password = USER_DB[client_id]
    response_data = f"{session_key_tgs.decode()}|{tgt.decode()}"

    return crypto_utils.encrypt(client_password, response_data)