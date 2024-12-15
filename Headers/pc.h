#ifndef PC_H
#define PC_H

#include "node.h"
#include "packet.h"
#include "datagenerator.h"
#include "router.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <QMutex>

class PC : public Node {
private:
    DataGenerator dataGenerator;           // For generating packet payloads
    std::string ipAddress;                 // The IP address of this PC
    std::vector<std::string> otherIPs;     // Known IPs of other network users
    int port;                              // Port to send data through
    std::unordered_map<int, std::unique_ptr<Packet>> sentPackets; // Track sent packets by sequence number
    QMutex mutex;                          // Mutex to ensure thread safety

public:
    // Constructor
    PC(const std::string& ip, int port, double shape, double scale, double rate, unsigned int seed);

    // Set known IPs of other users
    void setOtherIPs(const std::vector<std::string>& ips);

    // Generate and send packets based on the event control system
    void generateAndSendPackets(int numPackets, const std::string& targetIP, Router* gateway);

    // Connect to a router
    bool connectToRouter(Router* router);

    // Process incoming packets (optional for a PC)
    void processIncomingPacket(const Packet& packet);

    // Log sent packets
    void logPacket(const Packet& packet);

    // Retransmit packets
    void retransmitPacket(int sequenceNumber, Router* gateway);
};

#endif // PC_H
