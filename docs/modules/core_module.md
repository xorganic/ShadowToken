# Modulul Core - Manipularea Token-urilor și Syscalls

## Descriere
Modulul de bază (core) al ShadowToken este responsabil de manipularea token-urilor de acces pentru a obține privilegii ridicate și de executarea directă a syscall-urilor. Aceste funcționalități permit tool-ului să ocolească API-urile Windows monitorizate, escaladându-și astfel privilegiile în mod stealth.

## Funcții Principale

### 1. Manipularea Token-urilor
Funcțiile de manipulare a token-urilor permit ShadowToken să acceseze și să modifice token-uri de acces pentru procesele țintă, obținând astfel privilegii ridicate.

- **OpenProcessToken**: Deschide token-ul de acces pentru un proces specificat.
- **DuplicateTokenWithPrivileges**: Creează un duplicat al unui token existent, ajustând privilegiile pentru a permite accesul de tip SYSTEM.
- **EscalateToSystem**: Escalează privilegiile la nivel SYSTEM, permițând ShadowToken să ruleze cu cele mai ridicate permisiuni pe sistemul Windows.

#### Exemple de Utilizare
```cpp
// Exemplu: Deschiderea token-ului pentru procesul curent
HANDLE processToken;
OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &processToken);
```

### 2. Syscall Wrappers
Pentru a evita detectarea prin utilizarea funcțiilor API Windows monitorizate, ShadowToken folosește syscall-uri directe pentru a accesa resursele de sistem.

OpenProcessSyscall: Oferă acces direct la funcția de deschidere a proceselor, fără a depinde de API-ul Windows OpenProcess.
NtCreateSection: Creează o secțiune de memorie pentru injecții stealth, utilizată în tehnicile de injecție fileless.
DuplicateTokenExSyscall: Creează un token duplicat folosind syscall direct, evitând astfel monitorizarea.
Exemple de Utilizare
cpp
Copy code
// Exemplu: Deschiderea unui proces folosind syscall direct
HANDLE hProcess = OpenProcessSyscall(TOKEN_QUERY, FALSE, targetPID);
Structura Modulului
Structura fișierelor în modulul core include:

token_manipulation.cpp: Funcții pentru deschiderea și manipularea token-urilor de acces.
syscall_wrappers.cpp: Funcții care utilizează syscall-uri directe pentru a evita API-urile Windows.
Importanță în ShadowToken
Acest modul asigură că ShadowToken poate accesa procesele și resursele de sistem fără a declanșa alarme în soluțiile de securitate. Tehnicile de manipulare a token-urilor și de utilizare a syscall-urilor sunt fundamentale pentru funcționalitățile de escaladare a privilegiilor și injecție stealth.

##Recomandări
Pentru a optimiza utilizarea funcțiilor din acest modul:

Utilizați funcția EscalateToSystem doar atunci când sunt necesare permisiuni SYSTEM.
Folosiți syscall_wrappers în locul API-urilor standard pentru a evita monitorizarea și detectarea.

## Referințe
- [Documentația Microsoft pentru token-urile de acces](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-tokens)
- [Documentația Microsoft API Windows](https://learn.microsoft.com/en-us/windows/win32/api/)



