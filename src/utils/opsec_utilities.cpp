#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>

namespace ShadowToken {

    class OpsecUtilities {
    public:
        OpsecUtilities() = default;

        // Verifică dacă ShadowToken rulează într-un mediu virtualizat sau de analiză
        bool IsVirtualizedEnvironment() {
            std::vector<std::string> virtualizationIndicators = { "VBOX", "VMWARE", "XEN", "VIRTUAL" };
            char cpuInfo[0x20] = { 0 };

            // Folosește instrucțiunea CPUID pentru a obține informații despre CPU
            __cpuid(reinterpret_cast<int*>(cpuInfo), 0x40000000);
            for (const auto& indicator : virtualizationIndicators) {
                if (strstr(cpuInfo, indicator.c_str())) {
                    std::cout << "[Warning] Mediul virtualizat detectat: " << indicator << std::endl;
                    return true;
                }
            }

            std::cout << "[Info] Niciun semn de virtualizare detectat." << std::endl;
            return false;
        }

        // Verifică prezența unor procese cunoscute de monitorizare și analiză
        bool DetectAnalysisTools() {
            std::vector<std::wstring> analysisTools = { L"procmon.exe", L"wireshark.exe", L"fiddler.exe", L"sysmon.exe" };

            for (const auto& tool : analysisTools) {
                if (IsProcessRunning(tool)) {
                    std::wcout << "[Warning] Instrument de analiză detectat: " << tool << std::endl;
                    return true;
                }
            }

            std::cout << "[Info] Niciun instrument de analiză detectat." << std::endl;
            return false;
        }

        // Șterge urmele din sistem și eliberează resursele în caz de detectare
        void SelfCleanup() {
            std::cout << "[Info] Autocurățare activată." << std::endl;
            DeleteLogs();
            // Alte operațiuni de curățare (de exemplu, ștergerea fișierelor temporare)
            std::cout << "[Info] Autocurățarea a fost finalizată." << std::endl;
        }

    private:
        // Verifică dacă un proces specific este activ în sistem
        bool IsProcessRunning(const std::wstring& processName) {
            HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hSnapshot == INVALID_HANDLE_VALUE) return false;

            PROCESSENTRY32W processEntry;
            processEntry.dwSize = sizeof(PROCESSENTRY32W);

            bool found = false;
            if (Process32FirstW(hSnapshot, &processEntry)) {
                do {
                    if (_wcsicmp(processEntry.szExeFile, processName.c_str()) == 0) {
                        found = true;
                        break;
                    }
                } while (Process32NextW(hSnapshot, &processEntry));
            }

            CloseHandle(hSnapshot);
            return found;
        }

        // Șterge log-urile ShadowToken
        void DeleteLogs() {
            std::string logFilePath = "ShadowToken.log";  // Locația log-ului principal

            if (DeleteFileA(logFilePath.c_str())) {
                std::cout << "[Info] Log-urile au fost șterse." << std::endl;
            } else {
                std::cerr << "[Warning] Nu s-a putut șterge fișierul de log: " << logFilePath << std::endl;
            }
        }
    };
}
