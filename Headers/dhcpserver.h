#ifndef DHCP_SERVER_H
#define DHCP_SERVER_H

#include <string>
#include <map>
#include <stdexcept>
#include <iostream>
#include <sstream>

class DHCP {
public:
    // Constructor
    DHCP(const std::string& range, int asID);

    // Generates the next available IP address in the range
    std::string generateIPAddress();

    // Handles the DHCP DISCOVER phase
    std::string handleDiscover(int nodeId, const std::string& macAddress);

    // Saves the current DHCP allocations to a file
    void saveAllocationsToFile();

    // Assigns an IP address to a node
    std::string assignIP(int nodeID, const std::string& macAddress);

    // Static variable to hold IP counter across all instances
    static int ipCounter;

    // Method to reset the IP counter if needed
    static void resetIpCounter() { ipCounter = 1; }

private:
    std::string ipRange;  // Base range of IP addresses
    int asID;             // Autonomous system ID
    std::map<int, std::string> allocatedIPs;   // Map of node ID to allocated IP
    std::map<int, std::string> macAddressTable; // Map of node ID to MAC address
};

#endif // DHCP_SERVER_H
