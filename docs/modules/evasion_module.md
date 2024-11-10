# Modulul Evasion - Tehnici de Evaziune

## Descriere
Modulul de evaziune al ShadowToken include funcții și tehnici pentru a reduce probabilitatea de detectare și pentru a opera stealth în medii securizate. Evaziunea este realizată prin metode precum timing aleatoriu, selecția proceselor cu profil redus și tehnici de injecție stealth.

## Funcții Principale

### 1. RandomSleep
Introducere de întârzieri aleatorii între operațiuni pentru a evita detectarea comportamentală. RandomSleep simulează un comportament mai natural prin întârzieri variate.

- **Parametri**:
  - `minSleepMillis` - Timpul minim de așteptare în milisecunde.
  - `maxSleepMillis` - Timpul maxim de așteptare în milisecunde.
- **Returnează**: Niciunul.

```cpp
// Exemplu de utilizare
timingEvasion.RandomSleep(100, 500);
```
### 2. GradualSleep
Funcția introduce întârzieri progresive, crescând treptat timpul de așteptare pentru a simula un comportament mai natural și mai complex.

Parametri:
minSleepMillis - Timpul minim de așteptare în milisecunde.
maxSleepMillis - Timpul maxim de așteptare în milisecunde.
steps - Numărul de pași în care să crească progresiv timpul de așteptare.
Returnează: Niciunul.
```cpp
// Exemplu de utilizare
timingEvasion.GradualSleep(100, 500, 5);
```
### 3. IsHighProfileProcess
Verifică dacă un proces este considerat high-profile, cum ar fi lsass.exe sau svchost.exe, pentru a evita injectarea în procese intens monitorizate.

Parametri:
processName - Numele procesului de verificat.
Returnează: true dacă procesul este high-profile, false în caz contrar.
```cpp
// Exemplu de utilizare
bool avoidProcess = processAnalyzer.IsHighProfileProcess(L"lsass.exe");
```
### 4. SelectLowProfileProcess
Selectează un proces low-profile pentru injectare stealth. Aceasta este o funcție cheie pentru a reduce detectarea prin evitarea proceselor monitorizate.

Parametri: Niciunul.
Returnează: Numele procesului selectat pentru injectare.
```cpp
// Exemplu de utilizare
std::wstring targetProcess = processAnalyzer.SelectLowProfileProcess();
```
## Structura Modulului
Principalele fișiere din acest modul sunt:

* timing_evasion.cpp: Funcții pentru întârzieri aleatorii și progresive.
* process_analyzer.cpp: Funcții pentru analizarea proceselor și selecția target-urilor low-profile.

## Importanța Modulului Evasion
Modulul de evaziune este crucial pentru ShadowToken deoarece permite funcționarea în mod stealth, evitând detectarea prin monitorizarea comportamentală și prin selecția proceselor sigure. Prin combinarea întârzierilor aleatorii și a evitării proceselor high-profile, ShadowToken poate reduce semnificativ riscul de detecție.

### Recomandări de Utilizare
Utilizați RandomSleep și GradualSleep pentru a varia comportamentul, evitând tiparele repetitive.
Verificați întotdeauna procesele țintă cu IsHighProfileProcess și folosiți SelectLowProfileProcess pentru a alege procese mai puțin monitorizate.


