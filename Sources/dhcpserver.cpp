#include "dhcpserver.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream> // Add this header for std::ostringstream

using namespace std;
int DHCP::ipCounter = 0;  // Initialize the static variable

// Constructor
DHCP::DHCP(const std::string& range, int asID) : ipRange(range), asID(asID) {}

// Generates the next available IP address in the range
std::string DHCP::generateIPAddress() {
    // Increment IP counter for each new allocation
    int lastOctet = ipCounter++;
    if (lastOctet > 254) {
        throw std::runtime_error("IP pool exhausted in range: " + ipRange);
    }
    return ipRange + "." + std::to_string(lastOctet);
}

// Handles the DHCP DISCOVER phase
std::string DHCP::handleDiscover(int nodeId, const std::string& macAddress) {
    // Check if this node already has an allocated IP
    if (allocatedIPs.find(nodeId) == allocatedIPs.end()) {
        // Assign a new IP if not already allocated
        allocatedIPs[nodeId] = generateIPAddress();
        macAddressTable[nodeId] = macAddress;
    }
    // Return the allocated IP
    return allocatedIPs[nodeId];
}

// Saves the current DHCP allocations to a file
void DHCP::saveAllocationsToFile() {
    ofstream file("C:/Users/Entezar/dhcp_allocations.txt");
    if (file.is_open()) {
        if (allocatedIPs.empty()) {
            cout << "No IPs have been allocated. Nothing to save.\n";
        } else {
            for (const auto& entry : allocatedIPs) {
                file << "AS: " << asID
                     << ", Node ID: " << entry.first
                     << ", IP: " << entry.second
                     << ", MAC: " << macAddressTable[entry.first] << "\n";
            }
            cout << "DHCP allocations saved to 'dhcp_allocations.txt'\n";
        }
        file.close();
    } else {
        cerr << "Failed to open file for writing DHCP allocations.\n";
    }
}



std::string DHCP::assignIP(int nodeID, const std::string& macAddress) {
    if (allocatedIPs.find(nodeID) != allocatedIPs.end()) {
        return allocatedIPs[nodeID];
    }

    // Generate new IP within the specified range
    std::ostringstream ipStream;
    ipStream << ipRange.substr(0, ipRange.find_last_of('.') + 1) << ipCounter;
    std::string newIP = ipStream.str();

    if (ipCounter > 254) {  // Check for IP exhaustion (max 254 IPs in a range)
        throw std::runtime_error("IP pool exhausted for range: " + ipRange);
    }

    // Assign the new IP and update the allocated IPs map
    allocatedIPs[nodeID] = newIP;
    macAddressTable[nodeID] = macAddress;
    ipCounter++;  // Increment for next IP

    std::cout << "Assigned IP " << newIP << " to Node " << nodeID << " with MAC " << macAddress << "\n";
    return newIP;
}
