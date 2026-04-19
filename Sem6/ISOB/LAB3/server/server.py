from kerberos import crypto_utils
from kerberos.db import SERVICE_DB
from datetime import datetime

SERVICE_ID = "file_storage"
SERVER_SECRET = SERVICE_DB[SERVICE_ID]


def validate_request(ticket_encrypted, auth_encrypted):
    try:
        ticket_data = crypto_utils.decrypt(SERVER_SECRET, ticket_encrypted)
        client_id, session_key_service = ticket_data.split("|")

        auth_data = crypto_utils.decrypt(session_key_service, auth_encrypted)
        auth_client_id, timestamp_str = auth_data.split("|")

        request_time = datetime.strptime(timestamp_str, "%Y-%m-%d %H:%M:%S")
        time_diff = (datetime.now() - request_time).total_seconds()

        if abs(time_diff) > 300:
            print("[Server] Ошибка: Слишком старый запрос (Replay Attack?)")
            return False

        print(f"[Server] Успех! Доступ разрешен для {auth_client_id}")
        return True
    except Exception as e:
        print(f"[Server] Ошибка проверки: {e}")
        return False