#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

class Base64Converter {
private:
    static const std::string BASE64_CHARS;

public:
    static std::string encode(const std::string& input) {
        std::string output;
        int val = 0, valb = -6;

        for (unsigned char c : input) {
            val = (val << 8) + c;
            valb += 8;
            while (valb >= 0) {
                output.push_back(BASE64_CHARS[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }

        if (valb > -6) {
            output.push_back(BASE64_CHARS[((val << 8) >> (valb + 8)) & 0x3F]);
        }

        while (output.size() % 4) {
            output.push_back('=');
        }

        return output;
    }

    static std::string decode(const std::string& input) {
        std::string output;
        std::vector<int> T(256, -1);

        for (int i = 0; i < 64; i++) {
            T[BASE64_CHARS[i]] = i;
        }

        int val = 0, valb = -8;
        for (unsigned char c : input) {
            if (T[c] == -1) break;
            val = (val << 6) + T[c];
            valb += 6;
            if (valb >= 0) {
                output.push_back(char((val >> valb) & 0xFF));
                valb -= 8;
            }
        }

        return output;
    }
};

const std::string Base64Converter::BASE64_CHARS =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

class SocketServer {
private:
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr;

public:
    SocketServer() : serverSocket(INVALID_SOCKET) {}

    bool initialize() {
        // Инициализация Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        // Создание сокета
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return false;
        }

        // Настройка адреса сервера
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(8888);

        // Привязка сокета
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        // Прослушивание порта
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return false;
        }

        std::cout << "Server started on port 8888" << std::endl;
        return true;
    }

    void processCommand(SOCKET clientSocket, const std::string& command) {
        std::string response;

        // Формат команды: COMMAND:DATA
        size_t colonPos = command.find(':');
        if (colonPos == std::string::npos) {
            response = "ERROR:Invalid command format";
        }
        else {
            std::string cmd = command.substr(0, colonPos);
            std::string data = command.substr(colonPos + 1);

            if (cmd == "ENCODE") {
                response = "ENCODED:" + Base64Converter::encode(data);
            }
            else if (cmd == "DECODE") {
                response = "DECODED:" + Base64Converter::decode(data);
            }
            else if (cmd == "TEST") {
                response = "TEST:Server is working! Your data: " + data;
            }
            else {
                response = "ERROR:Unknown command";
            }
        }

        // Отправка ответа клиенту
        send(clientSocket, response.c_str(), response.length(), 0);
    }

    void run() {
        while (true) {
            std::cout << "Waiting for connections..." << std::endl;

            // Принятие соединения
            SOCKET clientSocket = accept(serverSocket, NULL, NULL);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                continue;
            }

            std::cout << "Client connected!" << std::endl;

            char buffer[1024];
            int bytesReceived;

            // Получение данных от клиента
            while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0)) > 0) {
                buffer[bytesReceived] = '\0';
                std::string command(buffer);

                std::cout << "Received command: " << command << std::endl;

                // Обработка специальной команды выхода
                if (command == "QUIT") {
                    std::cout << "Client requested disconnect" << std::endl;
                    break;
                }

                // Обработка команды
                processCommand(clientSocket, command);
            }

            if (bytesReceived == 0) {
                std::cout << "Client disconnected" << std::endl;
            }
            else if (bytesReceived == SOCKET_ERROR) {
                std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
            }

            closesocket(clientSocket);
        }
    }

    ~SocketServer() {
        if (serverSocket != INVALID_SOCKET) {
            closesocket(serverSocket);
        }
        WSACleanup();
    }
};

int main() {
    std::cout << "=== Base64 Encoding Service Server ===" << std::endl;
    std::cout << "Commands format: COMMAND:DATA" << std::endl;
    std::cout << "Available commands: ENCODE, DECODE, TEST, QUIT" << std::endl;

    SocketServer server;
    if (server.initialize()) {
        server.run();
    }

    return 0;
}