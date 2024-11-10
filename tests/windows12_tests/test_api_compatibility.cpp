#include <gtest/gtest.h>
#include <Windows.h>
#include "../../src/core/syscall_wrappers.cpp"
#include "../../src/privilege_escalation/token_manipulation.cpp"

namespace ShadowTokenTests {

    // Test pentru compatibilitatea funcției OpenProcess
    TEST(APICompatibilityTest, OpenProcess_Compatibility) {
        DWORD pid = GetCurrentProcessId();
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
        ASSERT_NE(processHandle, nullptr) << "Funcția OpenProcess nu este compatibilă.";
        
        if (processHandle) CloseHandle(processHandle);
    }

    // Test pentru compatibilitatea funcției OpenProcessToken
    TEST(APICompatibilityTest, OpenProcessToken_Compatibility) {
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
        ASSERT_NE(processHandle, nullptr) << "Funcția OpenProcess nu este compatibilă.";

        HANDLE tokenHandle;
        bool success = OpenProcessToken(processHandle, TOKEN_QUERY, &tokenHandle);
        ASSERT_TRUE(success) << "Funcția OpenProcessToken nu este compatibilă.";

        if (tokenHandle) CloseHandle(tokenHandle);
        if (processHandle) CloseHandle(processHandle);
    }

    // Test pentru compatibilitatea funcției DuplicateTokenEx
    TEST(APICompatibilityTest, DuplicateTokenEx_Compatibility) {
        HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, GetCurrentProcessId());
        ASSERT_NE(processHandle, nullptr) << "Funcția OpenProcess nu este compatibilă.";

        HANDLE tokenHandle;
        bool success = OpenProcessToken(processHandle, TOKEN_DUPLICATE, &tokenHandle);
        ASSERT_TRUE(success) << "Funcția OpenProcessToken nu este compatibilă.";

        HANDLE duplicatedToken;
        success = DuplicateTokenEx(tokenHandle, MAXIMUM_ALLOWED, nullptr, SecurityImpersonation, TokenPrimary, &duplicatedToken);
        ASSERT_TRUE(success) << "Funcția DuplicateTokenEx nu este compatibilă.";

        if (tokenHandle) CloseHandle(tokenHandle);
        if (duplicatedToken) CloseHandle(duplicatedToken);
        if (processHandle) CloseHandle(processHandle);
    }

    // Test pentru compatibilitatea funcției NtCreateSection (syscall direct)
    TEST(APICompatibilityTest, NtCreateSection_Compatibility) {
        HANDLE sectionHandle;
        LARGE_INTEGER maxSize;
        maxSize.QuadPart = 1024 * 1024;

        NTSTATUS status = NtCreateSection(&sectionHandle, SECTION_ALL_ACCESS, nullptr, &maxSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, nullptr);
        ASSERT_TRUE(NT_SUCCESS(status)) << "Funcția NtCreateSection (syscall direct) nu este compatibilă.";

        if (sectionHandle) CloseHandle(sectionHandle);
    }
}
