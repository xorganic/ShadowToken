#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <mutex>

namespace ShadowToken {

    enum class LogLevel {
        INFO,
        WARNING,
        ERROR
    };

    class Logger {
    public:
        // Constructorul primește calea către fișierul de log și inițializează mutex-ul pentru sincronizare
        Logger(const std::string& logFilePath) : logFilePath(logFilePath) {}

        // Logare centralizată cu mesaj și nivel specificat
        void Log(const std::string& message, LogLevel level = LogLevel::INFO) {
            std::lock_guard<std::mutex> lock(logMutex);
            std::ofstream logFile(logFilePath, std::ios::app);
            if (!logFile.is_open()) {
                std::cerr << "[Error] Nu s-a putut deschide fișierul de log: " << logFilePath << std::endl;
                return;
            }

            std::string timestamp = GetCurrentTimestamp();
            std::string levelStr = LogLevelToString(level);
            logFile << "[" << timestamp << "] [" << levelStr << "] " << message << std::endl;

            logFile.close();
        }

    private:
        std::string logFilePath;
        std::mutex logMutex;

        // Obține timestamp-ul curent pentru logare
        std::string GetCurrentTimestamp() {
            std::time_t now = std::time(nullptr);
            char buf[20];
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
            return std::string(buf);
        }

        // Conversie din LogLevel în string pentru logare
        std::string LogLevelToString(LogLevel level) {
            switch (level) {
                case LogLevel::INFO: return "INFO";
                case LogLevel::WARNING: return "WARNING";
                case LogLevel::ERROR: return "ERROR";
                default: return "UNKNOWN";
            }
        }
    };
}

