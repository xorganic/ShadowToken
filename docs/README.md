# ShadowToken

## General Description
ShadowToken is designed for privilege escalation and detection evasion in a stealthy manner. The tool relies on token manipulation and fileless injections, utilizing Windows APIs, syscalls, persistence techniques, C2 communication, and UAC bypass methods. ShadowToken enables operation in secure environments by carefully selecting processes and adjusting privileges to avoid detection solutions.

## Project Structure
The project structure includes the following essential modules:
- `core`: Token manipulation and syscall handling
- `api_wrappers`: Wrappers for compatibility with Windows APIs
- `privilege_escalation`: Stealth privilege escalation methods
- `evasion`: Advanced evasion techniques for reduced detection
- `persistence`: Fileless persistence and scheduled tasks
- `c2_communication`: Secure and stealthy C2 communication
- `injection`: Fileless injection techniques (Reflective DLL, APC Injection)

## Installation
To install and configure ShadowToken:
1. **Compile**:
    ```bash
    cd scripts
    ./build.sh
    ```
2. **Deployment**:
    ```bash
    ./deploy.sh
    ```
   The binary `ShadowToken.exe` and configuration files will be packaged in `ShadowToken_v2025.tar.gz`.

## Usage
1. **Initialization**:
    Launch `ShadowToken.exe` on the target system. Configuration files in the `config/` directory can be adjusted for evasion control and C2 settings.
2. **Custom Settings**:
    - Modify `targets.json` to specify preferred processes for injection.
    - Update `c2_config.json` for the C2 address and communication method.
3. **Persistence**:
    Configure persistence through `persistence_module.md`, using the Windows registry or Task Scheduler.

## Modules and Features
For more details about each module, refer to the documentation in `docs/modules/`. The documentation includes:
- `core_module.md`: Token manipulation and syscall handling
- `api_wrappers_module.md`: Windows API compatibility wrappers
- `privilege_escalation.md`: Privilege escalation techniques
- `evasion_module.md`: Advanced evasion techniques
- `persistence_module.md`: Persistence setup
- `c2_communication.md`: C2 configuration
- `injection_module.md`: Stealth fileless injections

## Examples
1. **Privilege Escalation**:
    - Configure `privileges.json` for the required privileges.
    - Use the `EscalateToSystem` function to gain SYSTEM-level access.

2. **Fileless Injections**:
    - Select the desired injection method in `evasion.json`.
    - Execute `Reflective DLL` or `APC Injection` for stealthy payload execution.

3. **C2 Communication**:
    - Configure `c2_config.json` with connection details.
    - ShadowToken will send beacons and receive commands from the C2 server.

## Authors and Contributions
ShadowToken is developed for red teaming, focusing on privilege escalation and detection evasion. Contributions and suggestions for optimizing features and supporting new Windows versions are welcome.

## Disclaimer
This tool is intended solely for ethical red teaming purposes in compliance with applicable laws and regulations. Unauthorized use is strictly prohibited.
