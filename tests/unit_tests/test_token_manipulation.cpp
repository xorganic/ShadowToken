#include <gtest/gtest.h>
#include "../../src/core/token_manipulation.cpp"

namespace ShadowTokenTests {

    // Testează funcția de deschidere a unui token de proces
    TEST(TokenManipulationTest, OpenProcessToken_ValidProcess) {
        DWORD pid = GetCurrentProcessId();
        ShadowToken::TokenManipulator tokenManipulator;
        
        HANDLE tokenHandle = tokenManipulator.OpenProcessToken(pid);
        ASSERT_NE(tokenHandle, nullptr) << "Token-ul nu a fost deschis pentru procesul curent.";

        if (tokenHandle) CloseHandle(tokenHandle);
    }

    // Testează deschiderea unui token de proces invalid
    TEST(TokenManipulationTest, OpenProcessToken_InvalidProcess) {
        DWORD invalidPid = 999999;  // PID inexistent
        ShadowToken::TokenManipulator tokenManipulator;

        HANDLE tokenHandle = tokenManipulator.OpenProcessToken(invalidPid);
        ASSERT_EQ(tokenHandle, nullptr) << "Token-ul a fost deschis incorect pentru un proces invalid.";
    }

    // Testează duplicarea unui token valid
    TEST(TokenManipulationTest, DuplicateTokenWithPrivileges_ValidToken) {
        DWORD pid = GetCurrentProcessId();
        ShadowToken::TokenManipulator tokenManipulator;

        HANDLE originalToken = tokenManipulator.OpenProcessToken(pid);
        ASSERT_NE(originalToken, nullptr) << "Token-ul nu a fost deschis pentru procesul curent.";

        HANDLE duplicatedToken = tokenManipulator.DuplicateTokenWithPrivileges(originalToken);
        ASSERT_NE(duplicatedToken, nullptr) << "Token-ul duplicat nu a fost creat cu succes.";

        if (originalToken) CloseHandle(originalToken);
        if (duplicatedToken) CloseHandle(duplicatedToken);
    }

    // Testează duplicarea unui token invalid
    TEST(TokenManipulationTest, DuplicateTokenWithPrivileges_InvalidToken) {
        ShadowToken::TokenManipulator tokenManipulator;
        HANDLE invalidToken = nullptr;

        HANDLE duplicatedToken = tokenManipulator.DuplicateTokenWithPrivileges(invalidToken);
        ASSERT_EQ(duplicatedToken, nullptr) << "Token-ul duplicat a fost creat incorect pentru un token invalid.";
    }
}
