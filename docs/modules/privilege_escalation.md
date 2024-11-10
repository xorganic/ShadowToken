# Modulul Privilege Escalation - Escaladarea Privilegiilor

## Descriere
Modulul de escaladare a privilegiilor din ShadowToken permite obținerea accesului de nivel SYSTEM și ocolirea restricțiilor UAC (User Account Control). Aceste tehnici sunt esențiale pentru a permite ShadowToken să acceseze resurse privilegiate, să injecteze payload-uri și să manipuleze procesele în mod stealth.

## Funcții Principale

### 1. EscalateToSystem
Această funcție obține token-ul de acces SYSTEM prin duplicarea token-ului unui proces cu privilegii ridicate, precum `winlogon.exe` sau alte procese de sistem.

- **Parametri**: Niciunul.
- **Returnează**: `true` dacă escaladarea a reușit, `false` în caz contrar.

```cpp
// Exemplu de utilizare
bool success = privilegeEscalator.EscalateToSystem();
```

### 2. DuplicateTokenWithPrivileges
Creează un duplicat al token-ului cu privilegii de SYSTEM, permițând executarea ShadowToken cu permisiuni administrative ridicate.
Parametri:
 * hExistingToken - Handle-ul token-ului curent.
 * desiredAccess - Permisiunile solicitate pentru token-ul duplicat.
Returnează: Un handle la token-ul duplicat.
```cpp
// Exemplu de utilizare
HANDLE hToken = DuplicateTokenWithPrivileges(existingToken, TOKEN_ALL_ACCESS);
```

### 3. BypassUAC_COMHijacking
Funcție pentru bypass UAC prin COM Hijacking, utilizată pentru a ocoli promptul UAC și a executa ShadowToken cu privilegii ridicate. COM Hijacking este o tehnică de deturnare a unor componente COM cu drepturi ridicate pentru a obține acces la nivel de administrator.

* Parametri: Niciunul.
* Returnează: true dacă bypass-ul reușește, false în caz contrar.
```cpp
// Exemplu de utilizare
bool bypassSuccess = privilegeEscalator.BypassUAC_COMHijacking();
```

## BypassUAC_EventViewer
Permite bypass-ul UAC prin deturnarea Event Viewer-ului pentru a lansa ShadowToken cu privilegii ridicate. Aceasta utilizează Event Viewer-ul Windows, care rulează cu drepturi ridicate, pentru a executa payload-ul ShadowToken.

Parametri: Niciunul.
Returnează: true dacă bypass-ul reușește, false în caz contrar.
```cpp
// Exemplu de utilizare
bool bypassSuccess = privilegeEscalator.BypassUAC_EventViewer();
```
## Structura Modulului
Principalele fișiere ale acestui modul sunt:

token_manipulation.cpp: Funcții pentru obținerea și duplicarea token-urilor de acces.
uac_bypass.cpp: Implementări pentru tehnici de bypass UAC, inclusiv COM Hijacking și Event Viewer Hijacking.
Importanța Modulului Privilege Escalation
Acest modul asigură că ShadowToken poate escalada privilegiile la nivel SYSTEM și poate executa payload-uri stealth cu drepturi ridicate. Tehnicile de bypass UAC sunt importante pentru a permite escaladarea fără intervenția utilizatorului și pentru a evita detectarea.

## Recomandări de Utilizare
Utilizați EscalateToSystem pentru a obține drepturi SYSTEM doar în momentele critice ale operațiunii.
Activează bypass-ul UAC în scenarii în care escaladarea este absolut necesară, fără a trezi suspiciuni.
## Referințe
[Documentație Microsoft pentru Privilege Escalation](https://learn.microsoft.com/en-us/windows/win32/secauthz/access-tokens)
[Mastering the Art of Bypassing Windows UAC](https://hadess.io/user-account-control-uncontrol-mastering-the-art-of-bypassing-windows-uac/)
