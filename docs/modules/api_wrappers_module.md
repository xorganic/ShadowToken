#  API Wrappers Module 
## Description
The API Wrappers module in ShadowToken is responsible for creating wrappers for Windows APIs, enabling the tool to use necessary functions in a compatible and stealthy manner. These wrappers help evade detection by avoiding direct calls to monitored APIs and abstracting critical Windows functions.

## Main Functions
### 1. OpenProcessSyscallWrapper
This wrapper allows ShadowToken to open a specified process via a direct syscall, bypassing the Windows OpenProcess API, which is frequently monitored by security solutions.

Parameters:
pid - The target process ID.
desiredAccess - The requested permissions for the process.
Returns: A handle to the target process if the operation is successful.
```cpp
// Example usage
HANDLE hProcess = OpenProcessSyscallWrapper(PROCESS_ALL_ACCESS, FALSE, targetPID);
```
### 2. CreateRemoteThreadWrapper
Allows creating a thread in a target process using syscalls to avoid detection. This is a key technique used for stealth injections such as Reflective DLL Injection.

```cpp
// Example usage
HANDLE hThread = CreateRemoteThreadWrapper(hProcess, lpStartAddress, lpParameter);
```
Parameters:
hProcess - Handle to the target process.
lpStartAddress - Start address of the thread.
lpParameter - Optional parameter for the thread function.
Returns: A handle to the created thread.
```cpp
Copy code
// Example usage
HANDLE hDuplicatedToken = DuplicateTokenExWrapper(hExistingToken, TOKEN_ALL_ACCESS);
```
## Module Structure
### The main files in this module are:

open_process_wrapper.cpp: Implementation of the syscall for the OpenProcess function.
create_remote_thread_wrapper.cpp: Wrapper for the CreateRemoteThread syscall.
duplicate_token_ex_wrapper.cpp: Implementation of the function for duplicating access tokens.
## Importance of API Wrappers
API wrappers are essential to ensure ShadowToken’s compatibility with modern operating systems and to enable the tool to use syscalls instead of direct calls to monitored APIs. These functions help reduce the risk of detection in environments with advanced security.

- [Microsoft Windows API Documentation](https://learn.microsoft.com/en-us/windows/win32/api/)
