#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

#define PIPE_NAME L"\\\\.\\pipe\\LogServerPipe"
#define BUFFER_SIZE 4096

// Мьютекс для синхронизации вывода
std::mutex cout_mutex;

void SafePrint(const std::string& message) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << message << std::endl;
}

class LogClient {
private:
    int clientId;

    bool ConnectWithRetry() {
        for (int attempt = 0; attempt < 3; attempt++) {
            HANDLE hPipe = CreateFile(
                PIPE_NAME,
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                0,
                NULL
            );

            if (hPipe != INVALID_HANDLE_VALUE) {
                return true;
            }

            DWORD error = GetLastError();
            if (error == ERROR_PIPE_BUSY) {
                SafePrint("Client " + std::to_string(clientId) + ": Pipe busy, retrying in 1s... (" +
                    std::to_string(attempt + 1) + "/3)");
                Sleep(1000);
            }
            else {
                SafePrint("Client " + std::to_string(clientId) + ": Could not open pipe. GLE=" + std::to_string(error));
                return false;
            }
        }
        SafePrint("Client " + std::to_string(clientId) + ": Failed to connect after 3 attempts");
        return false;
    }

public:
    LogClient(int id) : clientId(id) {}

    bool SendMessage(const std::string& message) {
        HANDLE hPipe = CreateFile(
            PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            SafePrint("Client " + std::to_string(clientId) + ": Could not open pipe. GLE=" + std::to_string(GetLastError()));
            return false;
        }

        // Режим сообщений
        DWORD dwMode = PIPE_READMODE_MESSAGE;
        BOOL fSuccess = SetNamedPipeHandleState(
            hPipe,
            &dwMode,
            NULL,
            NULL
        );

        if (!fSuccess) {
            SafePrint("Client " + std::to_string(clientId) + ": SetNamedPipeHandleState failed. GLE=" + std::to_string(GetLastError()));
            CloseHandle(hPipe);
            return false;
        }

        // Отправляем сообщение
        DWORD cbWritten;
        fSuccess = WriteFile(
            hPipe,
            message.c_str(),
            message.length() + 1,
            &cbWritten,
            NULL
        );

        if (!fSuccess) {
            SafePrint("Client " + std::to_string(clientId) + ": WriteFile failed. GLE=" + std::to_string(GetLastError()));
            CloseHandle(hPipe);
            return false;
        }

        SafePrint("Client " + std::to_string(clientId) + " sent: " + message);

        // Читаем ответ сервера
        char buffer[BUFFER_SIZE];
        DWORD cbRead;
        fSuccess = ReadFile(
            hPipe,
            buffer,
            BUFFER_SIZE - 1,
            &cbRead,
            NULL
        );

        if (!fSuccess) {
            DWORD error = GetLastError();
            if (error == ERROR_BROKEN_PIPE || error == ERROR_PIPE_NOT_CONNECTED) {
                SafePrint("Client " + std::to_string(clientId) + ": Server disconnected normally");
            }
            else {
                SafePrint("Client " + std::to_string(clientId) + ": ReadFile failed. GLE=" + std::to_string(error));
            }
        }
        else {
            buffer[cbRead] = '\0';
            SafePrint("Client " + std::to_string(clientId) + " received: " + buffer);
        }

        CloseHandle(hPipe);
        return true;
    }
};

void DemoClient(int clientId) {
    LogClient client(clientId);
    SafePrint("=== Client " + std::to_string(clientId) + " started ===");

    // Разные клиенты начинают с разной задержкой
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * clientId));

    for (int i = 1; i <= 3; i++) {
        std::string message = "Hello from client " + std::to_string(clientId) + " - message " + std::to_string(i);

        if (!client.SendMessage(message)) {
            SafePrint("Client " + std::to_string(clientId) + " failed to send message");
            break;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    // Отправляем сообщение о дисконнекте
    client.SendMessage("DISCONNECT");
    SafePrint("=== Client " + std::to_string(clientId) + " finished ===");
}

int main() {
    while (true) {
        std::cout << "=== Log Client Demo ===" << std::endl;
        std::cout << "0 - Exit" << std::endl;
        std::cout << "1 - Single client demo" << std::endl;
        std::cout << "2 - Multiple clients demo" << std::endl;
        std::cout << "Choose mode: ";

        std::string choice;
        std::cin >> choice;

        if (choice.size() != 1) {
            std::cout << "Invalid choice!" << std::endl;
            continue;
        }

        switch (choice[0]) {
        case '0':
            return 0;
        case '1':
            DemoClient(1);
            break;
        case '2': {
            std::thread client1(DemoClient, 1);
            std::thread client2(DemoClient, 2);

            client1.join();
            client2.join();
            break;
        }
        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    }
}