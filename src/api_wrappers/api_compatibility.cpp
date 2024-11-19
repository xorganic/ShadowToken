#include <Windows.h>
#include <iostream>

namespace ShadowToken {

    class APICompatibility {
    public:
        // Wrapper pentru deschiderea unui proces, cu verificarea compatibilității pe Windows 11/12
        HANDLE OpenProcessCompat(DWORD desiredAccess, BOOL inheritHandle, DWORD processID) {
            HANDLE processHandle = OpenProcess(desiredAccess, inheritHandle, processID);
            if (!processHandle) {
                DWORD error = GetLastError();
                if (error == ERROR_ACCESS_DENIED) {
                    std::cerr << "[Warning] Acces refuzat la procesul cu ID-ul " << processID << ". Verifică permisiunile sau restricțiile EDR." << std::endl;
                    // Încercați un fallback sau ajustați privilegii dacă este posibil
                    EnablePrivilege(L"SeDebugPrivilege");
                    processHandle = OpenProcess(desiredAccess, inheritHandle, processID);
                }
            }
            return processHandle;
        }

        // Wrapper pentru funcția OpenProcessToken cu fallback pentru Windows 11/12
        bool OpenProcessTokenCompat(HANDLE processHandle, DWORD desiredAccess, HANDLE* tokenHandle) {
            if (!OpenProcessToken(processHandle, desiredAccess, tokenHandle)) {
                DWORD error = GetLastError();
                if (error == ERROR_ACCESS_DENIED) {
                    std::cerr << "[Warning] Acces refuzat la token. Încercăm fallback..." << std::endl;
                    EnablePrivilege(L"SeDebugPrivilege");
                    return OpenProcessToken(processHandle, desiredAccess, tokenHandle);
                }
                return false;
            }
            return true;
        }

        // Activează un privilegiu pentru procesul curent (fallback în caz de eroare)
        bool EnablePrivilege(const std::wstring& privilege) {
            HANDLE token;
            if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token)) {
                std::cerr << "[Error] Nu s-a putut deschide token-ul procesului curent pentru ajustarea privilegiilor." << std::endl;
                return false;
            }

            TOKEN_PRIVILEGES tp;
            LUID luid;
            if (!LookupPrivilegeValueW(nullptr, privilege.c_str(), &luid)) {
                std::cerr << "[Error] Nu s-a găsit privilegiul specificat: " << GetLastError() << std::endl;
                CloseHandle(token);
                return false;
            }

            tp.PrivilegeCount = 1;
            tp.Privileges[0].Luid = luid;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), nullptr, nullptr)) {
                std::cerr << "[Error] Nu s-a putut ajusta privilegiul token-ului." << std::endl;
                CloseHandle(token);
                return false;
            }

            CloseHandle(token);
            return true;
        }
    };
}
