#ifndef MACADDRESSGENERATOR_H
#define MACADDRESSGENERATOR_H

#include <unordered_set>
#include <string>
#include <sstream>
#include <iomanip>
#include <random>
#include <iostream>


class GenerateMACAddress {
private:
    std::unordered_set<std::string> generatedMACs;
    std::string generateRandomMAC();

public:
    // Constructor
    GenerateMACAddress() = default;

    std::string generateUniqueMAC();
};

#endif // MACADDRESS_H
