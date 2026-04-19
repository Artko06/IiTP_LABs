import socket
import time
import config


def send_request(msg, label="[TEST]"):
    try:
        with socket.create_connection((config.HOST, config.PORT), timeout=3) as sock:
            sock.sendall(msg.encode())
            response = sock.recv(1024)
            print(f"{label} Ответ: {response.decode().strip()}")
    except Exception as e:
        print(f"{label} Ошибка: {e}")


def test_message_overflow():
    print("\n--- Тест: Превышение размера данных ---")
    big_data = "A" * (config.MAX_MESSAGE_SIZE + 1)
    send_request(big_data, label="[OVERFLOW]")


def test_slow_attack():
    print("\n--- Тест: Медленная отправка ---")
    try:
        with socket.create_connection((config.HOST, config.PORT)) as sock:
            print("[SLOW] Отправляем по одному символу слова 'Hi'...")
            sock.send(b"H")
            time.sleep(config.CONNECTION_TIMEOUT + 1)  # Ждем дольше таймаута сервера
            sock.send(b"i")
            # Сервер должен закрыть соединение до того, как мы дошлем "i"
            resp = sock.recv(1024)
            print(f"[SLOW] Ответ: {resp.decode()}")
    except Exception as e:
        print(f"[SLOW] Соединение разорвано сервером (как и ожидалось): {e}")


def test_rate_limiting():
    print(f"\n--- Тест: Flood (лимит {config.MAX_REQUESTS_PER_IP} запросов) ---")
    for i in range(config.MAX_REQUESTS_PER_IP + 2):
        print(f"[FLOOD] Запрос №{i + 1}")
        send_request("Ping", label=f"[FLOOD {i + 1}]")
        time.sleep(0.1)


def test_empty_connection():
    print("\n--- Тест: Пустое соединение (занятие порта) ---")
    try:
        with socket.create_connection((config.HOST, config.PORT)) as sock:
            print("[EMPTY] Подключились...")
            time.sleep(config.CONNECTION_TIMEOUT + 1)
            data = sock.recv(1024)
            if not data:
                print("[EMPTY] Сервер закрыл соединение по таймауту")
    except Exception as e:
        print(f"[EMPTY] Ошибка: {e}")


if __name__ == "__main__":
    send_request("Hello!", label="[NORMAL]")

    test_message_overflow()

    test_slow_attack()

    test_empty_connection()

    test_rate_limiting()