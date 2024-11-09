#include <gtest/gtest.h>
#include "../../src/core/syscall_wrappers.cpp"

namespace ShadowTokenTests {

    // Test pentru syscall-ul OpenProcessSyscall cu un PID valid
    TEST(SyscallWrappersTest, OpenProcessSyscall_ValidProcess) {
        DWORD pid = GetCurrentProcessId();
        ShadowToken::SyscallWrappers syscallWrapper;

        HANDLE processHandle = syscallWrapper.OpenProcessSyscall(pid, PROCESS_QUERY_INFORMATION);
        ASSERT_NE(processHandle, nullptr) << "Nu s-a putut deschide procesul curent folosind syscall direct.";

        if (processHandle) CloseHandle(processHandle);
    }

    // Test pentru syscall-ul OpenProcessSyscall cu un PID invalid
    TEST(SyscallWrappersTest, OpenProcessSyscall_InvalidProcess) {
        DWORD invalidPid = 999999;  // PID care nu există
        ShadowToken::SyscallWrappers syscallWrapper;

        HANDLE processHandle = syscallWrapper.OpenProcessSyscall(invalidPid, PROCESS_QUERY_INFORMATION);
        ASSERT_EQ(processHandle, nullptr) << "Syscall-ul a deschis incorect un proces inexistent.";
    }

    // Test pentru syscall-ul OpenProcessTokenSyscall cu un handle valid de proces
    TEST(SyscallWrappersTest, OpenProcessTokenSyscall_ValidProcess) {
        DWORD pid = GetCurrentProcessId();
        ShadowToken::SyscallWrappers syscallWrapper;

        HANDLE processHandle = syscallWrapper.OpenProcessSyscall(pid, PROCESS_QUERY_INFORMATION);
        ASSERT_NE(processHandle, nullptr) << "Nu s-a putut deschide procesul curent folosind syscall.";

        HANDLE tokenHandle = syscallWrapper.OpenProcessTokenSyscall(processHandle, TOKEN_QUERY);
        ASSERT_NE(tokenHandle, nullptr) << "Nu s-a putut deschide token-ul procesului curent prin syscall direct.";

        if (processHandle) CloseHandle(processHandle);
        if (tokenHandle) CloseHandle(tokenHandle);
    }

    // Test pentru syscall-ul OpenProcessTokenSyscall cu un handle invalid
    TEST(SyscallWrappersTest, OpenProcessTokenSyscall_InvalidProcess) {
        ShadowToken::SyscallWrappers syscallWrapper;
        HANDLE invalidHandle = nullptr;

        HANDLE tokenHandle = syscallWrapper.OpenProcessTokenSyscall(invalidHandle, TOKEN_QUERY);
        ASSERT_EQ(tokenHandle, nullptr) << "Syscall-ul a deschis incorect un token pentru un handle invalid.";
    }
}
