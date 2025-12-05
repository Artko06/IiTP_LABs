#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iphlpapi.h>
#include <psapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "user32.lib")

class SystemInfo {
private:
    struct OSInfo {
        std::string name;
        std::string version;
        std::string build;
        std::string architecture;
        std::string installDate;
    };

    struct CPUInfo {
        std::string name;
        std::string manufacturer;
        DWORD numberOfCores;
        DWORD numberOfLogicalProcessors;
        DWORD clockSpeed;
        std::string architecture;
    };

    struct MemoryInfo {
        ULONGLONG totalPhysical;
        ULONGLONG availablePhysical;
        ULONGLONG totalVirtual;
        ULONGLONG availableVirtual;
        DWORD memoryLoad;
    };

    struct DiskInfo {
        std::string drive;
        std::string fileSystem;
        ULONGLONG totalSize;
        ULONGLONG freeSpace;
        std::string volumeName;
    };

    struct GraphicsInfo {
        std::string cardName;
        DWORD horizontalResolution;
        DWORD verticalResolution;
        DWORD colorDepth;
    };

    struct NetworkInfo {
        std::string hostName;
        std::string userName;
        std::string macAddress;
    };

    OSInfo osInfo;
    CPUInfo cpuInfo;
    MemoryInfo memoryInfo;
    std::vector<DiskInfo> disksInfo;
    GraphicsInfo graphicsInfo;
    NetworkInfo networkInfo;
    std::vector<std::string> runningProcesses;

    std::string ReadRegistryString(HKEY hKey, const std::string& subKey, const std::string& valueName) {
        HKEY hRegistryKey;
        CHAR buffer[1024];
        DWORD bufferSize = sizeof(buffer);
        std::string result = "N/A";

        if (RegOpenKeyExA(hKey, subKey.c_str(), 0, KEY_READ, &hRegistryKey) == ERROR_SUCCESS) {
            if (RegQueryValueExA(hRegistryKey, valueName.c_str(), NULL, NULL, (LPBYTE)buffer, &bufferSize) == ERROR_SUCCESS) {
                result = buffer;
            }
            RegCloseKey(hRegistryKey);
        }
        return result;
    }

    std::string FormatBytes(ULONGLONG bytes) {
        const char* suffixes[] = { "B", "KB", "MB", "GB", "TB" };
        int suffixIndex = 0;
        double size = static_cast<double>(bytes);

        while (size >= 1024 && suffixIndex < 4) {
            size /= 1024;
            suffixIndex++;
        }

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << size << " " << suffixes[suffixIndex];
        return ss.str();
    }

    std::string GetMACAddress() {
        IP_ADAPTER_INFO adapterInfo[16];
        DWORD bufferSize = sizeof(adapterInfo);

        if (GetAdaptersInfo(adapterInfo, &bufferSize) == ERROR_SUCCESS) {
            PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
            if (pAdapterInfo) {
                std::stringstream ss;
                for (int i = 0; i < pAdapterInfo->AddressLength; i++) {
                    if (i == (pAdapterInfo->AddressLength - 1))
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)pAdapterInfo->Address[i];
                    else
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)pAdapterInfo->Address[i] << "-";
                }
                return ss.str();
            }
        }
        return "N/A";
    }

    void CollectOSInfo() {
        // Получаем версию ОС
        HMODULE hNtdll = GetModuleHandleA("ntdll.dll");
        if (hNtdll) {
            typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
            RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(hNtdll, "RtlGetVersion");

            if (RtlGetVersion) {
                RTL_OSVERSIONINFOW osVersion = { 0 };
                osVersion.dwOSVersionInfoSize = sizeof(osVersion);
                if (RtlGetVersion(&osVersion) == 0) {
                    osInfo.version = std::to_string(osVersion.dwMajorVersion) + "." +
                        std::to_string(osVersion.dwMinorVersion) + "." +
                        std::to_string(osVersion.dwBuildNumber);
                }
            }
        }

        osInfo.name = ReadRegistryString(HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName");

        osInfo.build = ReadRegistryString(HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild");

        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        osInfo.architecture = (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ? "64-bit" :
            (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) ? "32-bit" : "Unknown";

        std::string installTimestamp = ReadRegistryString(HKEY_LOCAL_MACHINE,
            "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "InstallDate");

        if (installTimestamp != "N/A") {
            try {
                time_t installTime = std::stol(installTimestamp);
                std::tm timeinfo;
                localtime_s(&timeinfo, &installTime);
                std::stringstream ss;
                ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
                osInfo.installDate = ss.str();
            }
            catch (...) {
                osInfo.installDate = "N/A";
            }
        }
        else {
            osInfo.installDate = "N/A";
        }
    }

    void CollectCPUInfo() {
        cpuInfo.name = ReadRegistryString(HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString");

        std::string vendor = ReadRegistryString(HKEY_LOCAL_MACHINE,
            "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "VendorIdentifier");

        if (vendor.find("GenuineIntel") != std::string::npos)
            cpuInfo.manufacturer = "Intel";
        else if (vendor.find("AuthenticAMD") != std::string::npos)
            cpuInfo.manufacturer = "AMD";
        else
            cpuInfo.manufacturer = vendor;

        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        cpuInfo.numberOfLogicalProcessors = sysInfo.dwNumberOfProcessors;

        HKEY hKey;
        DWORD mhz = 0;
        DWORD size = sizeof(DWORD);
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            RegQueryValueExA(hKey, "~MHz", NULL, NULL, (LPBYTE)&mhz, &size);
            RegCloseKey(hKey);
        }
        cpuInfo.clockSpeed = mhz;

        cpuInfo.architecture = (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ? "x64" :
            (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL) ? "x86" : "Unknown";

        DWORD cores = 0;
        size = sizeof(DWORD);
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
            0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            if (RegQueryValueExA(hKey, "NumberOfCores", NULL, NULL, (LPBYTE)&cores, &size) == ERROR_SUCCESS) {
                cpuInfo.numberOfCores = cores;
            }
            else {
                cpuInfo.numberOfCores = cpuInfo.numberOfLogicalProcessors / 2;
                if (cpuInfo.numberOfCores == 0) cpuInfo.numberOfCores = 1;
            }
            RegCloseKey(hKey);
        }
    }

    void CollectMemoryInfo() {
        MEMORYSTATUSEX memoryStatus;
        memoryStatus.dwLength = sizeof(memoryStatus);

        if (GlobalMemoryStatusEx(&memoryStatus)) {
            memoryInfo.totalPhysical = memoryStatus.ullTotalPhys;
            memoryInfo.availablePhysical = memoryStatus.ullAvailPhys;
            memoryInfo.totalVirtual = memoryStatus.ullTotalVirtual;
            memoryInfo.availableVirtual = memoryStatus.ullAvailVirtual;
            memoryInfo.memoryLoad = memoryStatus.dwMemoryLoad;
        }
    }

    void CollectDiskInfo() {
        DWORD driveMask = GetLogicalDrives();

        for (char drive = 'A'; drive <= 'Z'; drive++) {
            if (driveMask & 1) {
                std::string rootPath = std::string(1, drive) + ":\\";
                UINT driveType = GetDriveTypeA(rootPath.c_str());

                if (driveType == DRIVE_FIXED) {
                    DiskInfo disk;
                    disk.drive = rootPath;

                    CHAR fileSystem[32];
                    DWORD serialNumber, maxComponentLength, fileSystemFlags;

                    if (GetVolumeInformationA(rootPath.c_str(),
                        nullptr, 0, &serialNumber, &maxComponentLength,
                        &fileSystemFlags, fileSystem, sizeof(fileSystem))) {
                        disk.fileSystem = fileSystem;
                    }

                    ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
                    if (GetDiskFreeSpaceExA(rootPath.c_str(), &freeBytes, &totalBytes, &totalFreeBytes)) {
                        disk.totalSize = totalBytes.QuadPart;
                        disk.freeSpace = freeBytes.QuadPart;
                    }

                    CHAR volumeName[256];
                    if (GetVolumeInformationA(rootPath.c_str(), volumeName, sizeof(volumeName),
                        &serialNumber, &maxComponentLength, &fileSystemFlags, fileSystem, sizeof(fileSystem))) {
                        disk.volumeName = volumeName;
                    }

                    disksInfo.push_back(disk);
                }
            }
            driveMask >>= 1;
        }
    }

    void CollectGraphicsInfo() {
        DISPLAY_DEVICEA displayDevice;
        ZeroMemory(&displayDevice, sizeof(displayDevice));
        displayDevice.cb = sizeof(displayDevice);

        for (int i = 0; EnumDisplayDevicesA(NULL, i, &displayDevice, 0); i++) {
            if (displayDevice.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
                graphicsInfo.cardName = displayDevice.DeviceString;

                DEVMODEA devMode;
                devMode.dmSize = sizeof(devMode);
                if (EnumDisplaySettingsA(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode)) {
                    graphicsInfo.horizontalResolution = devMode.dmPelsWidth;
                    graphicsInfo.verticalResolution = devMode.dmPelsHeight;
                    graphicsInfo.colorDepth = devMode.dmBitsPerPel;
                }
                break;
            }
        }
    }

    void CollectNetworkInfo() {
        CHAR computerName[256];
        DWORD size = sizeof(computerName);
        if (GetComputerNameA(computerName, &size)) {
            networkInfo.hostName = computerName;
        }

        CHAR userName[256];
        size = sizeof(userName);
        if (GetUserNameA(userName, &size)) {
            networkInfo.userName = userName;
        }

        networkInfo.macAddress = GetMACAddress();
    }

    void CollectProcessesInfo() {
        DWORD processes[1024];
        DWORD needed;

        if (EnumProcesses(processes, sizeof(processes), &needed)) {
            DWORD processCount = needed / sizeof(DWORD);

            for (DWORD i = 0; i < processCount && runningProcesses.size() < 15; i++) {
                if (processes[i] != 0) {
                    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
                    if (hProcess) {
                        CHAR processName[MAX_PATH];
                        if (GetModuleBaseNameA(hProcess, NULL, processName, sizeof(processName))) {
                            runningProcesses.push_back(processName);
                        }
                        CloseHandle(hProcess);
                    }
                }
            }
        }
    }

    void CollectAllInfo() {
        CollectOSInfo();
        CollectCPUInfo();
        CollectMemoryInfo();
        CollectDiskInfo();
        CollectGraphicsInfo();
        CollectNetworkInfo();
        CollectProcessesInfo();
    }

public:
    SystemInfo() {
        CollectAllInfo();
    }

    void DisplayInfo() {
        std::cout << "==================================================" << std::endl;
        std::cout << "           SYSTEM INFORMATION UTILITY" << std::endl;
        std::cout << "==================================================" << std::endl << std::endl;

        // Операционная система
        std::cout << "=== OPERATING SYSTEM ===" << std::endl;
        std::cout << "OS Name:      " << osInfo.name << std::endl;
        std::cout << "Version:      " << osInfo.version << std::endl;
        std::cout << "Build:        " << osInfo.build << std::endl;
        std::cout << "Architecture: " << osInfo.architecture << std::endl;
        std::cout << "Install Date: " << osInfo.installDate << std::endl << std::endl;

        // Процессор
        std::cout << "=== PROCESSOR ===" << std::endl;
        std::cout << "CPU:          " << cpuInfo.name << std::endl;
        std::cout << "Manufacturer: " << cpuInfo.manufacturer << std::endl;
        std::cout << "Cores:        " << cpuInfo.numberOfCores << " physical, "
            << cpuInfo.numberOfLogicalProcessors << " logical" << std::endl;
        std::cout << "Clock Speed:  " << cpuInfo.clockSpeed << " MHz" << std::endl;
        std::cout << "Architecture: " << cpuInfo.architecture << std::endl << std::endl;

        // Память
        std::cout << "=== MEMORY ===" << std::endl;
        std::cout << "Total Physical:    " << FormatBytes(memoryInfo.totalPhysical) << std::endl;
        std::cout << "Available Physical: " << FormatBytes(memoryInfo.availablePhysical) << std::endl;
        std::cout << "Total Virtual:     " << FormatBytes(memoryInfo.totalVirtual) << std::endl;
        std::cout << "Available Virtual: " << FormatBytes(memoryInfo.availableVirtual) << std::endl;
        std::cout << "Memory Usage:      " << memoryInfo.memoryLoad << "%" << std::endl << std::endl;

        // Диски
        std::cout << "=== STORAGE ===" << std::endl;
        for (const auto& disk : disksInfo) {
            double usedPercent = 100.0 - ((double)disk.freeSpace / disk.totalSize * 100.0);
            std::cout << disk.drive << " (" << disk.volumeName << ")" << std::endl;
            std::cout << "  File System: " << disk.fileSystem << std::endl;
            std::cout << "  Total Size:  " << FormatBytes(disk.totalSize) << std::endl;
            std::cout << "  Free Space:  " << FormatBytes(disk.freeSpace) << std::endl;
            std::cout << "  Used:        " << std::fixed << std::setprecision(1) << usedPercent << "%" << std::endl << std::endl;
        }

        // Графика
        std::cout << "=== GRAPHICS ===" << std::endl;
        std::cout << "Graphics Card: " << graphicsInfo.cardName << std::endl;
        std::cout << "Resolution:    " << graphicsInfo.horizontalResolution << " x "
            << graphicsInfo.verticalResolution << std::endl;
        std::cout << "Color Depth:   " << graphicsInfo.colorDepth << " bits" << std::endl << std::endl;

        // Сеть
        std::cout << "=== NETWORK ===" << std::endl;
        std::cout << "Computer Name: " << networkInfo.hostName << std::endl;
        std::cout << "User Name:     " << networkInfo.userName << std::endl;
        std::cout << "MAC Address:   " << networkInfo.macAddress << std::endl << std::endl;

        // Процессы
        std::cout << "=== RUNNING PROCESSES (first 15) ===" << std::endl;
        for (size_t i = 0; i < runningProcesses.size(); i++) {
            std::cout << std::setw(20) << std::left << runningProcesses[i];
            if ((i + 1) % 3 == 0) std::cout << std::endl;
        }
        if (!runningProcesses.empty() && runningProcesses.size() % 3 != 0) {
            std::cout << std::endl;
        }
    }
};

int main() {
    std::setlocale(LC_ALL, "RU");

    std::cout << "Collecting system information..." << std::endl << std::endl;

    SystemInfo systemInfo;
    systemInfo.DisplayInfo();

    std::cout << std::endl << "Press Enter to exit...";
    std::cin.get();

    return 0;
}