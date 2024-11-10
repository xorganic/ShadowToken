# Modulul Persistence - Persistența în Sistem

## Descriere
Modulul de persistență al ShadowToken este responsabil de menținerea tool-ului activ pe sistemul țintă, asigurându-se că ShadowToken va rula automat la pornirea sistemului. Persistența este realizată prin adăugarea de task-uri programate sau prin configurarea registrului Windows.

## Funcții Principale

### 1. CreateScheduledTask
Creează un task programat în Task Scheduler-ul Windows pentru a lansa ShadowToken automat la logon-ul utilizatorului, asigurând persistența în mod stealth.

- **Parametri**:
  - `taskName` - Numele task-ului programat.
  - `exePath` - Calea către executabilul ShadowToken.
- **Returnează**: `true` dacă task-ul a fost creat cu succes, `false` în caz contrar.

```cpp
// Exemplu de utilizare
PersistenceTask persistence;
bool taskCreated = persistence.CreateScheduledTask(L"ShadowTokenTask", L"C:\\path\\to\\ShadowToken.exe");
```
### 2. RemoveScheduledTask
Elimină task-ul programat pentru persistență din Task Scheduler, oprind astfel lansarea automată a tool-ului la logon.

Parametri:
* taskName - Numele task-ului care trebuie eliminat.
Returnează: true dacă task-ul a fost eliminat cu succes, false în caz contrar.
```cpp
// Exemplu de utilizare
bool taskRemoved = persistence.RemoveScheduledTask(L"ShadowTokenTask");
```
### 3. SetupRegistryPersistence
Adaugă o cheie în registrul de startup Windows (HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run) pentru a lansa ShadowToken la fiecare pornire a sistemului.

Parametri:
* regPath - Calea din registru pentru configurarea persistenței.
* keyName - Numele cheii de registru.
* exePath - Calea către executabilul ShadowToken.
Returnează: true dacă persistenta prin registru a fost setată cu succes, false în caz contrar.
```cpp
// Exemplu de utilizare
bool registryPersistenceSet = persistence.SetupRegistryPersistence(
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", L"ShadowToken", L"C:\\path\\to\\ShadowToken.exe");
```
### 4. RemoveRegistryPersistence
Șterge cheia de registru pentru persistență din registrul Windows, oprind astfel lansarea automată a tool-ului la startup.

Parametri:
* regPath - Calea din registru de unde trebuie eliminată cheia.
* keyName - Numele cheii de registru care trebuie eliminată.
Returnează: true dacă cheia a fost eliminată cu succes, false în caz contrar.
```cpp
// Exemplu de utilizare
bool registryPersistenceRemoved = persistence.RemoveRegistryPersistence(
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", L"ShadowToken");
```
## Structura Modulului
Principalele fișiere din acest modul sunt:

* persistence_task.cpp: Funcții pentru configurarea persistenței prin Task Scheduler.
* persistence_registry.cpp: Funcții pentru configurarea persistenței prin registrul Windows.
Importanța Modulului Persistence
Persistența este crucială pentru ShadowToken deoarece permite tool-ului să rămână activ pe sistem chiar și după reporniri. Tehnicile de persistență stealth, precum task-urile programate și cheile de registru, asigură că ShadowToken se poate auto-lansa fără interacțiunea utilizatorului.

## Recomandări de Utilizare
Utilizați CreateScheduledTask pentru persistență pe sisteme moderne, unde accesul la Task Scheduler este permis.
În cazurile unde Task Scheduler este restricționat, folosiți SetupRegistryPersistence pentru a adăuga ShadowToken în registrul de startup.
[Documentație Microsoft pentru Task Scheduler](https://learn.microsoft.com/en-us/windows/win32/taskschd/task-scheduler-start-page)
[Persistență în Windows Registry](https://learn.microsoft.com/en-us/windows/win32/sysinfo/registry)
