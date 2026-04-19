import asyncio
import config
from security import security_mgr

async def handle_client(reader, writer):
    ip, port = writer.get_extra_info('peername')
    print(f"[*] Новое соединение: {ip}:{port}")

    if not security_mgr.is_allowed(ip):
        print(f"[!] Отказ в обслуживании для {ip}")
        writer.close()
        return

    try:
        data = await asyncio.wait_for(
            reader.read(config.MAX_MESSAGE_SIZE + 1),
            timeout=config.CONNECTION_TIMEOUT
        )

        if not security_mgr.validate_message_size(data):
            print(f"[!] Слишком большой объем данных от {ip}")
            writer.write(b"Error: Message too large\n")
            await writer.drain()
        else:
            message = data.decode().strip()
            print(f"[DATA] Получено от {ip}: {message}")
            writer.write(f"Echo: {message}\n".encode())
            await writer.drain()

    except asyncio.TimeoutError:
        print(f"[!] Таймаут соединения для {ip} (возможная атака)")
    except Exception as e:
        print(f"[!] Ошибка при обработке {ip}: {e}")
    finally:
        writer.close()
        await writer.wait_closed()

async def main():
    server = await asyncio.start_server(handle_client, config.HOST, config.PORT)
    print(f"[SERVER] Запущен на {config.HOST}:{config.PORT}")
    async with server:
        await server.serve_forever()

if __name__ == "__main__":
    asyncio.run(main())