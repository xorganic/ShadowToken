# ShadowToken

## Descriere generală
ShadowToken este un tool avansat de red teaming, proiectat pentru escaladarea privilegiilor și evaziunea detecției într-un mod stealth. Tool-ul se bazează pe manipularea token-urilor și injecțiile fileless, utilizând API-uri Windows, syscall-uri și tehnici de persistență, comunicare C2 și bypass UAC. ShadowToken permite operarea în medii securizate, evitând soluțiile de detectare prin selecția atentă a proceselor și ajustarea privilegiilor.

## Structura proiectului
Structura proiectului include modulele esențiale:
- `core`: Manipularea token-urilor și apelurile de syscall
- `api_wrappers`: Wrappere pentru compatibilitatea cu API-urile Windows
- `privilege_escalation`: Escaladarea privilegiilor prin metode stealth
- `evasion`: Tehnici avansate de evaziune pentru detectarea redusă
- `persistence`: Persistență fileless și task-uri programate
- `c2_communication`: Comunicare C2 securizată și stealth
- `injection`: Tehnici de injecție fileless (Reflective DLL, APC Injection)

## Instalare
Pentru a instala și configura ShadowToken:
1. **Compilare**:
    ```bash
    cd scripts
    ./build.sh
    ```
2. **Deployment**:
    ```bash
    ./deploy.sh
    ```
   Binarul `ShadowToken.exe` și fișierele de configurare vor fi incluse în `ShadowToken_v2025.tar.gz`.

## Utilizare
1. **Inițiere**:
    Lansați `ShadowToken.exe` pe sistemul țintă. Configurațiile din fișierele JSON (`config/`) pot fi ajustate pentru controlul evaziunii și al C2.
2. **Setări personalizate**:
    - Modificați `targets.json` pentru a specifica procesele preferate pentru injecție.
    - Actualizați `c2_config.json` pentru adresa și metoda de comunicare C2.
3. **Persistență**:
    Persistența poate fi configurată prin `persistence_module.md`, folosind registrul Windows sau Task Scheduler.

## Module și Funcționalități
Pentru detalii suplimentare despre fiecare modul, consultați fișierele din `docs/modules/`. Documentația include:
- `core_module.md`: Manipularea token-urilor și apelurile syscall
- `api_wrappers_module.md`: Wrappere pentru compatibilitatea API-urilor Windows
- `privilege_escalation.md`: Escaladarea privilegiilor
- `evasion_module.md`: Tehnici de evaziune avansată
- `persistence_module.md`: Setarea persistenței
- `c2_communication.md`: Configurare C2
- `injection_module.md`: Injecții fileless stealth

## Exemple
1. **Escaladare de privilegii**:
    - Configurați `privileges.json` pentru privilegiile necesare.
    - Utilizați funcția `EscalateToSystem` pentru a obține acces la nivel SYSTEM.

2. **Injecții fileless**:
    - Selectați metoda de injecție dorită în `evasion.json`.
    - Rulați injecția `Reflective DLL` sau `APC Injection` pentru a executa payload-uri stealth.

3. **Comunicație C2**:
    - Configurați `c2_config.json` pentru detalii de conexiune.
    - ShadowToken va transmite beacon-uri și poate primi comenzi de la serverul C2.

## Autori și Contribuții
ShadowToken este dezvoltat pentru red teaming, cu accent pe escaladarea privilegiilor și evaziunea detecției. Acceptăm contribuții și sugestii pentru optimizarea funcționalităților și suportul pentru noile versiuni Windows.

## Disclaimer
Acest tool este destinat exclusiv utilizării în scopuri etice de red teaming, conform legilor și regulamentelor în vigoare. Utilizarea neautorizată este strict interzisă.
