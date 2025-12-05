#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

class SocketClient {
private:
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

public:
    SocketClient() : clientSocket(INVALID_SOCKET) {}

    bool connectToServer(const std::string& serverIP, int port) {
        // Инициализация Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
            return false;
        }

        // Создание сокета
        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return false;
        }

        // Настройка адреса сервера
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

        // Подключение к серверу
        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Connect failed: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return false;
        }

        std::cout << "Connected to server " << serverIP << ":" << port << std::endl;
        return true;
    }

    std::string sendCommand(const std::string& command) {
        // Отправка команды серверу
        if (send(clientSocket, command.c_str(), command.length(), 0) == SOCKET_ERROR) {
            return "ERROR:Send failed";
        }

        // Получение ответа от сервера
        char buffer[1024];
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            return std::string(buffer);
        }
        else if (bytesReceived == 0) {
            return "ERROR:Connection closed";
        }
        else {
            return "ERROR:Receive failed";
        }
    }

    void interactiveMode() {
        std::string input;

        std::cout << "Interactive mode started. Type 'quit' to exit." << std::endl;
        std::cout << "Command format: COMMAND:DATA" << std::endl;
        std::cout << "Examples:" << std::endl;
        std::cout << "  ENCODE:Hello World" << std::endl;
        std::cout << "  DECODE:SGVsbG8gV29ybGQ=" << std::endl;
        std::cout << "  TEST:Some text" << std::endl;

        while (true) {
            std::cout << "\nEnter command: ";
            std::getline(std::cin, input);

            if (input == "quit" || input == "exit") {
                sendCommand("QUIT");
                break;
            }

            if (!input.empty()) {
                std::string response = sendCommand(input);
                std::cout << "Server response: " << response << std::endl;
            }
        }
    }

    ~SocketClient() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
        }
        WSACleanup();
    }
};

int main() {
    std::cout << "=== Base64 Encoding Service Client ===" << std::endl;

    SocketClient client;

    // Подключение к localhost на порт 8888
    if (client.connectToServer("127.0.0.1", 8888)) {
        client.interactiveMode();
    }
    else {
        std::cout << "Failed to connect to server. Make sure server is running." << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
    }

    return 0;
}