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

