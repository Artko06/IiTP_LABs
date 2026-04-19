from kerberos import crypto_utils
from kerberos.db import SERVICE_DB, TGS_SECRET_KEY
from cryptography.fernet import Fernet


def get_service_ticket(tgt_encrypted, service_id):
    if service_id not in SERVICE_DB:
        raise Exception("Сервис не существует")

    tgt_data = crypto_utils.decrypt(TGS_SECRET_KEY, tgt_encrypted)
    client_id, session_key_tgs = tgt_data.split("|")

    service_session_key = Fernet.generate_key()

    ticket_data = f"{client_id}|{service_session_key.decode()}"
    service_ticket = crypto_utils.encrypt(SERVICE_DB[service_id], ticket_data)

    response_data = f"{service_session_key.decode()}|{service_ticket.decode()}"
    return crypto_utils.encrypt(session_key_tgs, response_data)