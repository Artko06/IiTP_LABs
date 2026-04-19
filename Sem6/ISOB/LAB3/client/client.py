from server import server
from datetime import datetime
from kerberos import crypto_utils, tgs_service, as_service


def run_auth_flow():
    user = "artem_kokhan"
    password = "artem_kokhan_password_123"
    target_service = "file_storage"

    print(f"--- 1. Запрос к AS(Authentication Service) - Логин: {user} ---")
    try:
        as_resp = as_service.get_tgt(user)
        s_key_tgs, tgt = crypto_utils.decrypt(password, as_resp).split("|")
        print("TGT получен и расшифрован.")

        print(f"\n--- 2. Запрос к TGS(Ticket Granting Service) - Сервис: {target_service} ---")
        tgs_resp = tgs_service.get_service_ticket(tgt, target_service)
        s_key_service, service_ticket = crypto_utils.decrypt(s_key_tgs, tgs_resp).split("|")
        print("Билет к сервису получен.")

        print("\n--- 3. Обращение к Серверу ---")
        curr_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        authenticator = crypto_utils.encrypt(s_key_service, f"{user}|{curr_time}")

        if server.validate_request(service_ticket, authenticator):
            print("Результат: АУТЕНТИФИКАЦИЯ ПРОЙДЕНА")
        else:
            print("Результат: ОТКАЗАНО")

    except Exception as e:
        print(f"Произошла ошибка в kerberos: {e}")


if __name__ == "__main__":
    run_auth_flow()