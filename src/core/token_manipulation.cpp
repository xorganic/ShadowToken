#include <Windows.h>
#include <iostream>

namespace ShadowToken {

    class TokenManipulator {
    public:
        // Deschide un token al procesului specificat prin PID
        HANDLE OpenProcessToken(DWORD processID) {
            HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, processID);
            if (!processHandle) {
                std::cerr << "[Error] Nu s-a putut deschide procesul cu ID-ul " << processID << ": " << GetLastError() << std::endl;
                return nullptr;
            }

            HANDLE tokenHandle;
            if (!::OpenProcessToken(processHandle, TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY | TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenHandle)) {
                std::cerr << "[Error] Nu s-a putut deschide token-ul pentru procesul cu ID-ul " << processID << ": " << GetLastError() << std::endl;
                CloseHandle(processHandle);
                return nullptr;
            }

            CloseHandle(processHandle);
            return tokenHandle;
        }

        // Duplicați un token și ajustați-l pentru a acorda privilegii SYSTEM
        HANDLE DuplicateTokenWithPrivileges(HANDLE sourceToken) {
            HANDLE duplicatedToken;
            SECURITY_ATTRIBUTES secAttr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

            if (!DuplicateTokenEx(sourceToken, MAXIMUM_ALLOWED, &secAttr, SecurityImpersonation, TokenPrimary, &duplicatedToken)) {
                std::cerr << "[Error] Nu s-a putut duplica token-ul: " << GetLastError() << std::endl;
                return nullptr;
            }

            AdjustTokenPrivilegesForSystem(duplicatedToken);

            return duplicatedToken;
        }

    private:
        // Ajustează privilegiile token-ului pentru a include SeDebugPrivilege
        void AdjustTokenPrivilegesForSystem(HANDLE token) {
            TOKEN_PRIVILEGES privileges;
            LUID luid;

            if (!LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &luid)) {
                std::cerr << "[Error] Nu s-a putut găsi valoarea pentru privilegiu: " << GetLastError() << std::endl;
                return;
            }

            privileges.PrivilegeCount = 1;
            privileges.Privileges[0].Luid = luid;
            privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

            if (!AdjustTokenPrivileges(token, FALSE, &privileges, sizeof(privileges), nullptr, nullptr)) {
                std::cerr << "[Error] Nu s-a putut ajusta privilegiile token-ului: " << GetLastError() << std::endl;
            }
        }
    };
}
