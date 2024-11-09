#include <Windows.h>
#include <iostream>
#include <thread>
#include <random>
#include <chrono>

namespace ShadowToken {

    class TimingEvasion {
    public:
        TimingEvasion() = default;

        // Întârziere randomizată între două valori specifice
        void RandomSleep(int minMillis = 100, int maxMillis = 1000) {
            int delay = GenerateRandomDelay(minMillis, maxMillis);
            std::cout << "[Evasion] Întârziere aleatorie de " << delay << " ms pentru evaziune." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        // Întârziere graduală pentru a simula latența umană
        void GradualSleep(int startMillis = 100, int endMillis = 1000, int steps = 5) {
            int stepDelay = (endMillis - startMillis) / steps;
            for (int i = 0; i < steps; ++i) {
                int delay = startMillis + (i * stepDelay);
                std::cout << "[Evasion] Întârziere graduală de " << delay << " ms (pasul " << i + 1 << " din " << steps << ")." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
        }

    private:
        // Generează o întârziere aleatorie în milisecunde între minMillis și maxMillis
        int GenerateRandomDelay(int minMillis, int maxMillis) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(minMillis, maxMillis);
            return dis(gen);
        }
    };
}
