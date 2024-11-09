#include <gtest/gtest.h>
#include <chrono>
#include "../../src/evasion/timing_evasion.cpp"
#include "../../src/process_enumeration/process_analyzer.cpp"

namespace ShadowTokenTests {

    // Test pentru funcția de evaziune prin timing aleatoriu
    TEST(EvasionMethodsTest, RandomSleep) {
        ShadowToken::TimingEvasion timingEvasion;
        
        auto start = std::chrono::high_resolution_clock::now();
        timingEvasion.RandomSleep(100, 500);  // Interval de 100-500 ms
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        ASSERT_GE(duration, 100) << "Evaziunea RandomSleep a avut o întârziere mai mică decât cea specificată.";
        ASSERT_LE(duration, 500) << "Evaziunea RandomSleep a avut o întârziere mai mare decât cea specificată.";
    }

    // Test pentru funcția de evaziune prin întârziere graduală
    TEST(EvasionMethodsTest, GradualSleep) {
        ShadowToken::TimingEvasion timingEvasion;

        auto start = std::chrono::high_resolution_clock::now();
        timingEvasion.GradualSleep(100, 500, 5);  // 5 pași între 100 și 500 ms
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        ASSERT_GE(duration, 100) << "Evaziunea GradualSleep a avut o întârziere mai mică decât cea specificată.";
        ASSERT_LE(duration, 500 * 5) << "Evaziunea GradualSleep a avut o întârziere mai mare decât cea specificată.";
    }

    // Test pentru evitarea proceselor high-profile
    TEST(EvasionMethodsTest, AvoidHighProfileProcesses) {
        ShadowToken::ProcessAnalyzer processAnalyzer;
        
        // Setăm un proces high-profile și verificăm dacă este evitat
        std::wstring highProfileProcess = L"lsass.exe";
        bool shouldAvoid = processAnalyzer.IsHighProfileProcess(highProfileProcess);
        
        ASSERT_TRUE(shouldAvoid) << "Procesul high-profile nu a fost evitat corespunzător.";
    }

    // Test pentru selecția unui proces low-profile pentru injecție
    TEST(EvasionMethodsTest, SelectLowProfileProcess) {
        ShadowToken::ProcessAnalyzer processAnalyzer;

        // Așteptăm ca funcția să selecteze un proces low-profile adecvat
        std::wstring lowProfileProcess = processAnalyzer.SelectLowProfileProcess();
        ASSERT_FALSE(lowProfileProcess.empty()) << "Nu s-a selectat niciun proces low-profile pentru injecție.";
    }
}
