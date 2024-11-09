#include <gtest/gtest.h>
#include "../../src/fileless_injection/reflective_dll_injection.cpp"
#include "../../src/fileless_injection/apc_injection.cpp"

namespace ShadowTokenTests {

    // Test pentru injecția DLL reflexivă într-un proces țintă
    TEST(FilelessInjectionTest, ReflectiveDllInjection_Success) {
        ShadowToken::ReflectiveDLLInjector dllInjector;

        // PID-ul procesului țintă (de exemplu, un proces de testare)
        DWORD targetPID = GetCurrentProcessId();
        std::wstring payloadPath = L"path/to/payload.dll";

        // Verifică dacă injecția DLL reflexivă reușește
        bool injectionSuccess = dllInjector.InjectDLL(OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetPID), payloadPath);
        ASSERT_TRUE(injectionSuccess) << "Injecția DLL reflexivă a eșuat.";
    }

    // Test pentru injecția APC într-un proces țintă
    TEST(FilelessInjectionTest, ApcInjection_Success) {
        ShadowToken::APCInjector apcInjector;

        // PID-ul procesului țintă (de exemplu, un proces de testare)
        DWORD targetPID = GetCurrentProcessId();
        std::wstring payloadPath = L"path/to/payload.dll";

        // Verifică dacă injecția APC reușește
        bool injectionSuccess = apcInjector.InjectAPC(targetPID, payloadPath);
        ASSERT_TRUE(injectionSuccess) << "Injecția APC a eșuat.";
    }
}
