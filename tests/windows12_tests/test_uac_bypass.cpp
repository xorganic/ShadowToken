#include <gtest/gtest.h>
#include "../../src/privilege_escalation/uac_bypass.cpp"

namespace ShadowTokenTests {

    // Test pentru bypass-ul UAC folosind metoda COM Hijacking
    TEST(UACBypassTest, COMHijacking_Success) {
        ShadowToken::UACBypass uacBypass;

        // Verifică dacă bypass-ul UAC prin COM Hijacking reușește
        bool bypassSuccess = uacBypass.BypassUAC_COMHijacking();
        ASSERT_TRUE(bypassSuccess) << "Bypass-ul UAC prin COM Hijacking a eșuat.";
    }

    // Test pentru bypass-ul UAC folosind metoda Event Viewer Hijacking
    TEST(UACBypassTest, EventViewerHijacking_Success) {
        ShadowToken::UACBypass uacBypass;

        // Verifică dacă bypass-ul UAC prin Event Viewer Hijacking reușește
        bool bypassSuccess = uacBypass.BypassUAC_EventViewer();
        ASSERT_TRUE(bypassSuccess) << "Bypass-ul UAC prin Event Viewer Hijacking a eșuat.";
    }

    // Test pentru bypass-ul UAC folosind metoda de DLL Hijacking pe Windows 11+
    TEST(UACBypassTest, DLLHijacking_Success) {
        ShadowToken::UACBypass uacBypass;

        // Verifică dacă bypass-ul UAC prin DLL Hijacking reușește pe Windows 11+
        bool bypassSuccess = uacBypass.BypassUAC_DLLHijacking();
        ASSERT_TRUE(bypassSuccess) << "Bypass-ul UAC prin DLL Hijacking a eșuat pe Windows 11+.";
    }

    // Test pentru bypass-ul UAC pe un proces cu restricții stricte de securitate
    TEST(UACBypassTest, BypassRestrictedProcess_Failure) {
        ShadowToken::UACBypass uacBypass;

        // Test pentru bypass-ul UAC într-un proces cu restricții stricte, care ar trebui să eșueze
        bool bypassSuccess = uacBypass.BypassUAC_RestrictedProcess();
        ASSERT_FALSE(bypassSuccess) << "Bypass-ul UAC a reușit incorect pe un proces cu restricții stricte.";
    }
}
