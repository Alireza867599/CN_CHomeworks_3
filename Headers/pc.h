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
class Router;

class PC : public Node {
private:
    DataGenerator dataGenerator; // Member without a default constructor
    std::string ipAddress;                 // The IP address of this PC
    std::vector<std::string> otherIPs;     // Known IPs of other network users
    int port;                              // Port to send data through
    std::unordered_map<int, std::unique_ptr<Packet>> sentPackets; // Track sent packets by sequence number
    QMutex mutex;                          // Mutex to ensure thread safety
    std::vector<std::shared_ptr<Port>> m_ports; // Container to store port
    int PCID;
    std::string macaddress;                 // The IP address of this PC

public:
    // Constructor
    PC();
    std::vector<std::shared_ptr<Port>> getPorts() const; // Declaration of getPorts()

    explicit PC(int id); // Constructor that accepts an int

    void requestIP(shared_ptr<Router> router);

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

    void addPort(std::shared_ptr<Port> port);

    std::string getIpAddress() const;
    std::string getMacaddress() const;
    void setMacaddress(const std::string &newMacaddress);
};

#endif // PC_H
