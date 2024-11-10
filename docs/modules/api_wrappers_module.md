# Modulul API Wrappers - Wrapper-e pentru API-urile Windows

## Descriere
Modulul API Wrappers din ShadowToken este responsabil de crearea unor wrapper-e pentru API-urile Windows, permițând tool-ului să utilizeze funcțiile necesare într-un mod compatibil și stealth. Aceste wrapper-e ajută la evitarea detectării prin evitarea apelurilor directe către API-urile monitorizate și prin abstractizarea unor funcții Windows critice.

## Funcții Principale

### 1. OpenProcessSyscallWrapper
Acest wrapper permite lui ShadowToken să deschidă un proces specificat prin intermediul unui syscall direct, ocolind astfel API-ul Windows `OpenProcess`, care este monitorizat frecvent de soluțiile de securitate.

- **Parametri**:
  - `pid` - ID-ul procesului țintă.
  - `desiredAccess` - Permisiunile solicitate asupra procesului.
- **Returnează**: Un handle la procesul țintă, dacă operațiunea reușește.

```cpp
// Exemplu de utilizare
HANDLE hProcess = OpenProcessSyscallWrapper(PROCESS_ALL_ACCESS, FALSE, targetPID);
```
### 2. CreateRemoteThreadWrapper
Permite crearea unui thread într-un proces țintă folosind syscall-uri pentru a evita detectarea. Aceasta este o tehnică cheie utilizată pentru injecțiile stealth, cum ar fi Reflective DLL Injection.
```cpp
// Exemplu de utilizare
HANDLE hThread = CreateRemoteThreadWrapper(hProcess, lpStartAddress, lpParameter);
```
### Parametri:
hProcess - Handle-ul procesului țintă.
lpStartAddress - Adresa de start a thread-ului.
lpParameter - Parametru opțional pentru funcția thread-ului.
Returnează: Un handle la thread-ul creat.

```cpp
// Exemplu de utilizare
HANDLE hDuplicatedToken = DuplicateTokenExWrapper(hExistingToken, TOKEN_ALL_ACCESS);
```
## Structura Modulului
Fișierele principale din acest modul sunt:
open_process_wrapper.cpp: Implementarea syscall-ului pentru funcția OpenProcess.
create_remote_thread_wrapper.cpp: Wrapper pentru syscall-ul CreateRemoteThread.
duplicate_token_ex_wrapper.cpp: Implementarea funcției pentru duplicarea token-urilor de acces.
## Importanța Wrapper-elor API
Wrapper-ele API sunt esențiale pentru a asigura compatibilitatea ShadowToken pe sistemele moderne de operare și pentru a permite tool-ului să utilizeze syscall-uri în locul apelurilor directe către API-urile monitorizate. Astfel, aceste funcții ajută la reducerea riscului de detectare în medii cu securitate avansată.

[Windows API Documentation]([https://learn.microsoft.com/en-us/windows/win32/secauthz/access-tokens](https://learn.microsoft.com/en-us/windows/win32/api/)
