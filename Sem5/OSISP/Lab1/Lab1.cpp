#include <iostream>
#include <windows.h>
#include <chrono>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

CRITICAL_SECTION coutCS;
HANDLE hConsole;
const long long numThreads = 6;

int threadPriorities[numThreads] = {
    THREAD_PRIORITY_TIME_CRITICAL,    // 15
    THREAD_PRIORITY_HIGHEST,          // 2
    THREAD_PRIORITY_NORMAL,           // 0
    THREAD_PRIORITY_BELOW_NORMAL,     // -1
    THREAD_PRIORITY_LOWEST,           // -2
    THREAD_PRIORITY_IDLE              // -15
};

string priorityNames[numThreads] = {
    "TIME_CRITICAL (15)",
    "HIGHEST (2)",
    "NORMAL (0)",
    "BELOW_NORMAL (-1)",
    "LOWEST (-2)",
    "IDLE (-15)"
};

WORD threadColors[numThreads] = {
    FOREGROUND_RED | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

void UpdateProgressBar(long long index, long long progress)
{
    EnterCriticalSection(&coutCS);
    SetConsoleTextAttribute(hConsole, threadColors[index]);

    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = (SHORT)(index);
    SetConsoleCursorPosition(hConsole, cursorPosition);

    string progressBar = "Поток " + to_string(index + 1) + " [" + priorityNames[index].substr(0, 10) + "] : [";
    long long barWidth = 40;
    long long pos = barWidth * progress / 100;
    for (long long i = 0; i < barWidth; ++i) {
        if (i < pos) progressBar += "=";
        else progressBar += " ";
    }
    progressBar += "] " + to_string(progress) + "%";

    cout << progressBar;
    LeaveCriticalSection(&coutCS);
}

void MultiplyMatrices(const vector<vector<long double>>& A, const vector<vector<long double>>& B,
    vector<vector<long double>>& C, long long threadIndex)
{
    long long N = A.size();
    for (long long i = 0; i < N; ++i) {
        for (long long j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (long long k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
        long long progress = (100 * (i + 1)) / N;
        UpdateProgressBar(threadIndex, progress);
    }
}

DWORD WINAPI ThreadFunction(LPVOID lpParam)
{
    long long threadIndex = *(long long*)lpParam;
    SetThreadPriority(GetCurrentThread(), threadPriorities[threadIndex]);

    auto startTime = high_resolution_clock::now();

    // Увеличим размер матриц для более заметной разницы
    vector<vector<long double>> A(300, vector<long double>(300, 3.0));
    vector<vector<long double>> B(300, vector<long double>(300, 3.0));
    vector<vector<long double>> C(300, vector<long double>(300, 0.0));

    MultiplyMatrices(A, B, C, threadIndex);

    auto endTime = high_resolution_clock::now();
    duration<double> elapsed = endTime - startTime;

    EnterCriticalSection(&coutCS);
    SetConsoleTextAttribute(hConsole, threadColors[threadIndex]);

    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = (SHORT)(numThreads + threadIndex);
    SetConsoleCursorPosition(hConsole, cursorPosition);

    cout << "Поток " << threadIndex + 1 << " [" << priorityNames[threadIndex]
        << "] завершен за " << elapsed.count() << " сек.";

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    LeaveCriticalSection(&coutCS);

    return 0;
}

void RunTest(DWORD_PTR affinityMask)
{
    system("cls");

    int activeCores = 0;
    DWORD_PTR mask = affinityMask;
    while (mask) {
        activeCores += (mask & 1);
        mask >>= 1;
    }

    string title = "Test: " + to_string(activeCores) + " core/cores";
    SetConsoleTitleA(title.c_str());

    // Устанавливаем affinity для процесса
    SetProcessAffinityMask(GetCurrentProcess(), affinityMask);

    InitializeCriticalSection(&coutCS);
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    HANDLE hThreads[numThreads];
    DWORD threadIDs[numThreads];
    long long threadIndices[numThreads];

    // Создаем потоки
    for (long long i = 0; i < numThreads; ++i) {
        threadIndices[i] = i;
        hThreads[i] = CreateThread(NULL, 0, ThreadFunction, &threadIndices[i], 0, &threadIDs[i]);
    }

    // Ждем завершения
    WaitForMultipleObjects(numThreads, hThreads, TRUE, INFINITE);

    // Закрываем handles
    for (long long i = 0; i < numThreads; ++i) {
        CloseHandle(hThreads[i]);
    }

    DeleteCriticalSection(&coutCS);

    if (affinityMask != 0xFFFF) {
        cout << endl << "Тест завершен. Нажмите Enter для продолжения...";
        cin.get();
    }
}

int main()
{
    setlocale(LC_ALL, "RU");
    system("color 07");

    // Тест 1: 1 ядро
    RunTest(0x01);

    // Тест 2: 3 ядра
    RunTest(0x07);

    // Тест 3: 16 ядер
    RunTest(0xFFFF);

    return 0;
}