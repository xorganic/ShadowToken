#include <Windows.h>
#include <iostream>

extern "C" NTSTATUS NTAPI NtOpenProcess(
    PHANDLE ProcessHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PCLIENT_ID ClientId
);

extern "C" NTSTATUS NTAPI NtOpenProcessToken(
    HANDLE ProcessHandle,
    ACCESS_MASK DesiredAccess,
    PHANDLE TokenHandle
);

namespace ShadowToken {

    class SyscallWrappers {
    public:
        SyscallWrappers() = default;

        // Wrapper pentru deschiderea unui proces prin syscall direct
        HANDLE OpenProcessSyscall(DWORD processID, ACCESS_MASK desiredAccess) {
            HANDLE processHandle = nullptr;
            OBJECT_ATTRIBUTES objAttr = { sizeof(OBJECT_ATTRIBUTES) };
            CLIENT_ID clientId = { (HANDLE)processID, nullptr };

            NTSTATUS status = NtOpenProcess(&processHandle, desiredAccess, &objAttr, &clientId);
            if (status != 0) {
                std::cerr << "[Error] Nu s-a putut deschide procesul cu syscall: " << std::hex << status << std::endl;
                return nullptr;
            }

            return processHandle;
        }

        // Wrapper pentru obținerea token-ului unui proces folosind syscall direct
        HANDLE OpenProcessTokenSyscall(HANDLE processHandle, ACCESS_MASK desiredAccess) {
            HANDLE tokenHandle = nullptr;

            NTSTATUS status = NtOpenProcessToken(processHandle, desiredAccess, &tokenHandle);
            if (status != 0) {
                std::cerr << "[Error] Nu s-a putut deschide token-ul procesului cu syscall: " << std::hex << status << std::endl;
                return nullptr;
            }

            return tokenHandle;
        }
    };
}
