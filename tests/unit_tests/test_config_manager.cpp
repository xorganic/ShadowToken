#include <gtest/gtest.h>
#include "../../src/utils/config_manager.cpp"

namespace ShadowTokenTests {

    // Testează încărcarea configurației dintr-un fișier JSON valid
    TEST(ConfigManagerTest, LoadConfig_ValidFile) {
        ShadowToken::ConfigManager configManager("config/valid_config.json");
        ASSERT_TRUE(configManager.LoadConfig()) << "Configurația validă nu a fost încărcată corect.";
    }

    // Testează încărcarea unui fișier JSON invalid
    TEST(ConfigManagerTest, LoadConfig_InvalidFile) {
        ShadowToken::ConfigManager configManager("config/invalid_config.json");
        ASSERT_FALSE(configManager.LoadConfig()) << "Configurația invalidă a fost încărcată incorect.";
    }

    // Testează obținerea listei de procese high-profile
    TEST(ConfigManagerTest, GetHighProfileProcesses) {
        ShadowToken::ConfigManager configManager("config/valid_config.json");
        configManager.LoadConfig();
        
        auto highProfileProcesses = configManager.GetHighProfileProcesses();
        ASSERT_GT(highProfileProcesses.size(), 0) << "Lista de procese high-profile nu a fost încărcată corect.";
        ASSERT_EQ(highProfileProcesses[0], L"lsass.exe") << "Primul proces high-profile este incorect.";
    }

    // Testează obținerea intervalului de beacon pentru C2
    TEST(ConfigManagerTest, GetC2Interval) {
        ShadowToken::ConfigManager configManager("config/valid_config.json");
        configManager.LoadConfig();
        
        int interval = configManager.GetC2Interval();
        ASSERT_EQ(interval, 60) << "Intervalul de beacon C2 nu a fost încărcat corect.";
    }

    // Testează obținerea metodei C2
    TEST(ConfigManagerTest, GetC2Method) {
        ShadowToken::ConfigManager configManager("config/valid_config.json");
        configManager.LoadConfig();
        
        std::string method = configManager.GetC2Method();
        ASSERT_EQ(method, "http") << "Metoda C2 nu a fost încărcată corect.";
    }
    
    // Testează obținerea domeniului pentru DNS C2
    TEST(ConfigManagerTest, GetC2Address) {
        ShadowToken::ConfigManager configManager("config/valid_config.json");
        configManager.LoadConfig();
        
        std::string c2Address = configManager.GetC2Address();
        ASSERT_EQ(c2Address, "c2.example.com") << "Adresa C2 nu a fost încărcată corect.";
    }
}
