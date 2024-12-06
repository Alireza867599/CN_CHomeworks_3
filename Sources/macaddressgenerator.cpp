#include "macaddressgenerator.h"


std::string GenerateMACAddress::generateRandomMAC() {
    std::ostringstream macStream;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);

    for (int i = 0; i < 6; ++i) {
        int octet = dist(gen);

        if (i == 0) {
            octet = (octet & 0xFC) | 0x02; // Set the locally administered bit and clear multicast bit
        }

        macStream << std::hex << std::setw(2) << std::setfill('0') << octet;
        if (i < 5) {
            macStream << ":";
        }
    }

    return macStream.str();
}

std::string GenerateMACAddress::generateUniqueMAC() {
    std::string mac;

    // Generate until a unique MAC is found
    do {
        mac = generateRandomMAC();
    } while (generatedMACs.find(mac) != generatedMACs.end());

    // Add the MAC to the set
    generatedMACs.insert(mac);

    return mac;
}
