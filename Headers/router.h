#ifndef ROUTER_H
#define ROUTER_H
#include "packet.h"
#include "Node.h"
#include <queue>
#include <map>
#include <stack>
#include <vector>
#include <mutex>
#include <string>
#include <iostream>
#include <queue>
#include <map>
#include <vector>
#include <mutex>
#include <string>


struct RoutingTableEntry {
    std::string destinationNetwork; // e.g., 192.168.1.0
    std::string subnetMask;         // e.g., 255.255.255.0
    std::string gateway;            // Next hop
    int metric;                     // Cost or distance
    std::string protocol;           // e.g., RIP, OSPF, BGP
};

struct BufferManagementPolicy {
    enum Method { FIFO, LIFO, RED };
    Method method;
};

class Router : public Node {
private:
    // Routing Table
    std::vector<RoutingTableEntry> routingTable;

    // Buffer
    std::queue<Packet> fifoBuffer;
    std::stack<Packet> lifoBuffer;
    std::queue<Packet> buffer; // Common buffer for packets

    std::mutex bufferMutex;
    static constexpr size_t BUFFER_CAPACITY = 100;
    BufferManagementPolicy bufferPolicy = {BufferManagementPolicy::FIFO};

    // General properties
    int id;
    std::string ipAddress;
    bool isDhcpEnabled;
    bool isGatewayAS;
    bool isGatewayUser;
    static constexpr int PORT_COUNT = 4;
    std::map<int, Router *> connectedRouters; // Port to router mapping
    std::map<int, std::vector<Node *>> connectedUsers; // Port to user mapping


public:
    Router(int id, const std::string &ipAddress, const std::string &macAddress);
    void sendPacket(const Packet &packet);
    void setIpAddress(const std::string &newIpAddress);
    bool getIsDhcpEnabled() const;
    void setIsDhcpEnabled(bool newIsDhcpEnabled);
    bool getIsGatewayAS() const;
    void setIsGatewayAS(bool newIsGatewayAS);
    bool getIsGatewayUser() const;
    void setIsGatewayUser(bool newIsGatewayUser);

    // Routing Table Management
    void addRoutingTableEntry(const RoutingTableEntry &entry);
    void removeRoutingTableEntry(const std::string &destinationNetwork);
    void displayRoutingTable() const;
    RoutingTableEntry findBestRoute(const std::string &destination) const;

    // Buffer Management
    void setBufferPolicy(BufferManagementPolicy::Method method);
    void sendPacketToBuffer(const Packet &packet);
    Packet retrievePacketFromBuffer();

    // Packet processing and router-specific methods
    void enableDhcp();
    void setAsGatewayAS();
    void setAsGatewayUser();
    bool connectRouter(int port, Router *router);
    bool connectUser(int port, Node *user);
    void processBuffer();

    // Getter methods
    int getId() const;
    std::string getIpAddress() const;

};

#endif // ROUTER_H
