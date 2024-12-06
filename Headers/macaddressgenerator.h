#ifndef MACADDRESSGENERATOR_H
#define MACADDRESSGENERATOR_H

#include <unordered_set>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>

class GenerateMACAddress {
private:
    std::unordered_set<std::string> generatedMACs; // Set to store unique MAC addresses

    // Helper function to generate a single MAC address
    std::string generateRandomMAC();

public:
    // Constructor
    GenerateMACAddress() = default;

    // Method to generate a unique MAC address
    std::string generateUniqueMAC();
};

#endif // MACADDRESS_H
