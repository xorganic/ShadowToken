#include <gtest/gtest.h>
#include "../../src/persistence/persistence_manager.cpp"

namespace ShadowTokenTests {

    // Test pentru setarea persistenței prin registrul de startup
    TEST(PersistenceTest, RegistryPersistence_Setup) {
        ShadowToken::PersistenceManager persistenceManager;
        
        // Calea și cheia de registry pentru startup
        std::wstring regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
        std::wstring keyName = L"ShadowToken";
        std::wstring exePath = L"C:\\path\\to\\ShadowToken.exe";

        // Verifică dacă persistența este setată corect în registru
        bool persistenceSet = persistenceManager.SetupRegistryPersistence(regPath, keyName, exePath);
        ASSERT_TRUE(persistenceSet) << "Setarea persistenței în registru a eșuat.";
    }

    // Test pentru ștergerea persistenței din registrul de startup
    TEST(PersistenceTest, RegistryPersistence_Remove) {
        ShadowToken::PersistenceManager persistenceManager;

        // Calea și cheia de registry pentru startup
        std::wstring regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
        std::wstring keyName = L"ShadowToken";

        // Verifică dacă persistența este eliminată corect din registru
        bool persistenceRemoved = persistenceManager.RemoveRegistryPersistence(regPath, keyName);
        ASSERT_TRUE(persistenceRemoved) << "Ștergerea persistenței din registru a eșuat.";
    }

    // Test pentru setarea persistenței prin programarea unui task în Task Scheduler
    TEST(PersistenceTest, ScheduledTaskPersistence_Setup) {
        ShadowToken::PersistenceManager persistenceManager;

        // Numele task-ului și calea către executabilul ShadowToken
        std::wstring taskName = L"ShadowTokenTask";
        std::wstring exePath = L"C:\\path\\to\\ShadowToken.exe";

        // Verifică dacă task-ul programat este setat corect
        bool taskSet = persistenceManager.SetupScheduledTaskPersistence(taskName, exePath);
        ASSERT_TRUE(taskSet) << "Setarea persistenței prin Task Scheduler a eșuat.";
    }

    // Test pentru ștergerea persistenței prin eliminarea unui task programat
    TEST(PersistenceTest, ScheduledTaskPersistence_Remove) {
        ShadowToken::PersistenceManager persistenceManager;

        // Numele task-ului programat
        std::wstring taskName = L"ShadowTokenTask";

        // Verifică dacă task-ul programat este eliminat corect
        bool taskRemoved = persistenceManager.RemoveScheduledTaskPersistence(taskName);
        ASSERT_TRUE(taskRemoved) << "Ștergerea persistenței din Task Scheduler a eșuat.";
    }
}
