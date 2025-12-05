#include <iostream>
#include <windows.h>
#include <string>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>

#define PIPE_NAME L"\\\\.\\pipe\\LogServerPipe"
#define BUFFER_SIZE 4096

class LogServer {
private:
    std::ofstream logFile;
    int clientCounter;

    std::string GetCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::tm timeInfo;
        localtime_s(&timeInfo, &time_t);  

        std::stringstream ss;
        ss << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S");
        ss << "." << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }

    void WriteToLog(const std::string& clientId, const std::string& message) {
        std::string timestamp = GetCurrentTimestamp();
        std::string logEntry = "[" + timestamp + "] [Client:" + clientId + "] " + message + "\n";

        logFile << logEntry;
        logFile.flush();

        std::cout << logEntry;
    }

    HANDLE CreateNamedPipeInstance() {
        return CreateNamedPipe(
            PIPE_NAME,                      // pipe name 
            PIPE_ACCESS_DUPLEX,             // read/write access 
            PIPE_TYPE_MESSAGE |             // message type pipe 
            PIPE_READMODE_MESSAGE |         // message-read mode 
            PIPE_WAIT,                      // blocking mode 
            PIPE_UNLIMITED_INSTANCES,       // max. instances  
            BUFFER_SIZE,                    // output buffer size 
            BUFFER_SIZE,                    // input buffer size 
            NMPWAIT_USE_DEFAULT_WAIT,       // client time-out 
            NULL                            // default security attribute
        );
    }

    void HandleClient(HANDLE hPipe, const std::string& clientId) {
        char buffer[BUFFER_SIZE];
        DWORD bytesRead;
        BOOL success;

        while (true) {
            success = ReadFile(
                hPipe,                      // handle to pipe 
                buffer,                     // buffer to receive data 
                BUFFER_SIZE - 1,            // size of buffer 
                &bytesRead,                 // number of bytes read 
                NULL                        // not overlapped I/O 
            );

            if (!success || bytesRead == 0) {
                if (GetLastError() == ERROR_BROKEN_PIPE) {
                    std::cout << "Client " << clientId << " disconnected" << std::endl;
                    WriteToLog(clientId, "Client disconnected");
                }
                else {
                    std::cerr << "ReadFile failed, GLE=" << GetLastError() << std::endl;
                }
                break;
            }

            buffer[bytesRead] = '\0';
            std::string message(buffer);

            if (message == "DISCONNECT") {
                std::cout << "Client " << clientId << " disconnected normally" << std::endl;
                WriteToLog(clientId, "Client disconnected");
                break;
            }

            WriteToLog(clientId, message);

            // Send response back to client
            std::string response = "ACK: Message received";
            DWORD bytesWritten;
            BOOL fSuccess = WriteFile(
                hPipe,                      // handle to pipe 
                response.c_str(),           // buffer to write from 
                response.length() + 1,      // number of bytes to write, include NULL
                &bytesWritten,              // number of bytes written 
                NULL                        // not overlapped I/O 
            );

            if (!fSuccess) {
                std::cerr << "WriteFile failed, GLE=" << GetLastError() << std::endl;
                break;
            }
        }

        FlushFileBuffers(hPipe);
        DisconnectNamedPipe(hPipe);
        CloseHandle(hPipe);
    }

public:
    LogServer() : clientCounter(0) {
        logFile.open("server_log.txt", std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "ERROR: Cannot open log file!" << std::endl;
        }
        else {
            std::string timestamp = GetCurrentTimestamp();
            logFile << "\n=== Log Server started at " << timestamp << " ===\n";
            logFile.flush();
        }
    }

    ~LogServer() {
        if (logFile.is_open()) {
            logFile << "=== Log Server stopped ===\n";
            logFile.close();
        }
    }

    void Run() {
        std::cout << "Log Server started. Waiting for clients..." << std::endl;
        WriteToLog("SERVER", "Log server started");

        while (true) {
            HANDLE hPipe = CreateNamedPipeInstance();

            if (hPipe == INVALID_HANDLE_VALUE) {
                std::cerr << "ERROR: CreateNamedPipe failed, GLE=" << GetLastError() << std::endl;
                Sleep(2000);
                continue;
            }

            std::cout << "Waiting for client connection..." << std::endl;

            BOOL connected = ConnectNamedPipe(hPipe, NULL);
            if (connected) {
                clientCounter++;
                std::string clientId = std::to_string(clientCounter);
                std::cout << "Client " << clientId << " connected successfully!" << std::endl;
                WriteToLog(clientId, "Client connected");

                HandleClient(hPipe, clientId);
            }
            else {
                DWORD error = GetLastError();
                if (error == ERROR_PIPE_CONNECTED) {
                    clientCounter++;
                    std::string clientId = std::to_string(clientCounter);
                    std::cout << "Client " << clientId << " connected (already connected)!" << std::endl;
                    WriteToLog(clientId, "Client connected");

                    HandleClient(hPipe, clientId);
                }
                else {
                    std::cerr << "ConnectNamedPipe failed, GLE=" << error << std::endl;
                    CloseHandle(hPipe);
                }
            }
        }
    }
};

int main() {
    std::cout << "=== Log Server Application ===" << std::endl;

    LogServer server;
    server.Run();

    return 0;
}