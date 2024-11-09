#include <gtest/gtest.h>
#include "../../src/privilege_escalation/privilege_escalation.cpp"

namespace ShadowTokenTests {

    // Test pentru verificarea escaladării la token-ul SYSTEM
    TEST(PrivilegeEscalationTest, EscalateToSystem_Success) {
        ShadowToken::PrivilegeEscalator privilegeEscalator({"lsass.exe", "csrss.exe"});
        
        // Încearcă escaladarea la SYSTEM
        bool escalationSuccess = privilegeEscalator.EscalateToSystem();
        ASSERT_TRUE(escalationSuccess) << "Escaladarea la token-ul SYSTEM a eșuat.";
    }

    // Test pentru escaladare pe un proces invalid
    TEST(PrivilegeEscalationTest, EscalateToSystem_InvalidProcess) {
        ShadowToken::PrivilegeEscalator privilegeEscalator({"invalid_process.exe"});

        // Încearcă escaladarea, care ar trebui să eșueze deoarece procesul nu este valid
        bool escalationSuccess = privilegeEscalator.EscalateToSystem();
        ASSERT_FALSE(escalationSuccess) << "Escaladarea a reușit în mod incorect pe un proces invalid.";
    }

    // Test pentru verificarea obținerii unui token duplicat cu privilegii ridicate
    TEST(PrivilegeEscalationTest, DuplicateSystemToken_ValidToken) {
        DWORD pid = GetCurrentProcessId();
        ShadowToken::TokenManipulator tokenManipulator;

        HANDLE processToken = tokenManipulator.OpenProcessToken(pid);
        ASSERT_NE(processToken, nullptr) << "Nu s-a putut deschide token-ul procesului curent.";

        HANDLE duplicatedToken = tokenManipulator.DuplicateTokenWithPrivileges(processToken);
        ASSERT_NE(duplicatedToken, nullptr) << "Token-ul duplicat cu privilegii ridicate nu a fost creat corect.";

        if (processToken) CloseHandle(processToken);
        if (duplicatedToken) CloseHandle(duplicatedToken);
    }

    // Test pentru obținerea token-ului SYSTEM pe un proces monitorizat
    TEST(PrivilegeEscalationTest, EscalateToSystem_HighProfileProcess) {
        ShadowToken::PrivilegeEscalator privilegeEscalator({"svchost.exe", "explorer.exe"});
        
        // Acest test ar trebui să evite escaladarea pe procese monitorizate
        bool escalationSuccess = privilegeEscalator.EscalateToSystem();
        ASSERT_FALSE(escalationSuccess) << "Escaladarea a reușit pe un proces high-profile, ceea ce nu este dorit.";
    }
}
