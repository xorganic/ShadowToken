#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace ShadowToken {

    class ConfigManager {
    public:
        ConfigManager(const std::string& configFilePath)
            : configFilePath(configFilePath) {}

        bool LoadConfig() {
            std::ifstream configFile(configFilePath);
            if (!configFile.is_open()) {
                std::cerr << "[Error] Nu s-a putut deschide fișierul de configurare: " << configFilePath << std::endl;
                return false;
            }

            try {
                configFile >> configData;
            } catch (const json::parse_error& e) {
                std::cerr << "[Error] Eroare de parsare a JSON-ului: " << e.what() << std::endl;
                return false;
            }

            std::cout << "[Info] Configurația a fost încărcată cu succes." << std::endl;
            return true;
        }

        // Returnează lista proceselor high-profile de evitat
        std::vector<std::wstring> GetHighProfileProcesses() const {
            std::vector<std::wstring> highProfileProcesses;
            if (configData.contains("highProfileProcesses")) {
                for (const auto& process : configData["highProfileProcesses"]) {
                    highProfileProcesses.push_back(std::wstring(process.begin(), process.end()));
                }
            }
            return highProfileProcesses;
        }

        // Returnează intervalul pentru beaconing C2
        int GetC2Interval() const {
            if (configData.contains("c2Interval")) {
                return configData["c2Interval"].get<int>();
            }
            return 30;  // Valoare implicită în secunde
        }

        // Returnează metoda C2 configurată (ex. "http", "dns")
        std::string GetC2Method() const {
            if (configData.contains("c2Method")) {
                return configData["c2Method"].get<std::string>();
            }
            return "http";  // Valoare implicită
        }

        // Returnează adresa C2 configurată
        std::string GetC2Address() const {
            if (configData.contains("c2Address")) {
                return configData["c2Address"].get<std::string>();
            }
            return "localhost";  // Valoare implicită
        }

    private:
        std::string configFilePath;
        json configData;
    };
}
