#ifndef ROUTER_H
#define ROUTER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <iostream>
#include <mutex>
#include <memory>
#include <algorithm>
#include "Packet.h"
#include "dhcpserver.h"
#include "node.h"
#include "port.h"

#define BUFFER_CAPACITY 100
#define PORT_COUNT 6

extern std::shared_ptr<DHCP> dhcpServer;

class PC;

struct RoutingTableEntry {
    std::string destinationNetwork;
    std::string subnetMask;
    std::string gateway;
    int metric;
    std::string protocol;
};

struct BufferManagementPolicy {
    enum Method { FIFO, LIFO, RED };
    Method method;
};

class Router : public Node {
private:
    std::string dhcpIpRange;                 // IP range for DHCP
    int dhcpAsId = -1;                       // AS ID associated with this DHCP server

    std::string ipAddress;
    bool isDhcpEnabled;
    bool isGatewayAS;
    bool isGatewayUser;
    std::unique_ptr<DHCP> dhcpServer;
    std::vector<RoutingTableEntry> routingTable;
    BufferManagementPolicy bufferPolicy;
    std::map<int, std::string> dhcpAllocations;
    std::queue<Packet> fifoBuffer;
    std::stack<Packet> lifoBuffer;
    std::queue<Packet> buffer;
    std::vector<std::shared_ptr<Router>> connectedRouters;
    std::vector<std::shared_ptr<PC>> connectedPCs;
    mutable std::mutex bufferMutex;
    int routerId;
    int asId;
    std::vector<std::shared_ptr<Port>> m_ports;
    std::string macaddress;
public:
    Router(int id , const std::string& ipAddress,const std::string& macaddress, int asid);
    int getId() const;
    int getAsId() const;
    void addPort(std::shared_ptr<Port> port);
    std::string processDhcpDiscover(int nodeId, const std::string& macAddress);
    void saveDhcpAllocations();
    void enableDhcp(const std::string& ipRange, int asID);
    void handleIPRequest(std::shared_ptr<Node> node);
    void requestIP(std::shared_ptr<Router> router);

    void addRoutingTableEntry(const RoutingTableEntry& entry);
    void removeRoutingTableEntry(const std::string& destinationNetwork);
    void displayRoutingTable() const;
    RoutingTableEntry findBestRoute(const std::string& destination) const;

    void setBufferPolicy(BufferManagementPolicy::Method method);
    void sendPacketToBuffer(const Packet& packet);
    Packet retrievePacketFromBuffer();

    void setAsGatewayAS();
    void setAsGatewayUser();
    std::vector<std::shared_ptr<Port>> getPorts() const;

    bool connectRouter(int routerId, std::shared_ptr<Router> router);
    void connectUser(int pcId, std::shared_ptr<PC> pc);
    const std::vector<std::shared_ptr<PC>>& getConnectedPCs() const;
    std::vector<std::shared_ptr<Router>> getConnectedRouters() const;

    void sendPacket(const Packet& packet);
    void processBuffer();

    bool getIsDhcpEnabled() const;
    void setIsDhcpEnabled(bool newIsDhcpEnabled);
    std::string getMacaddress() const;
    void setMacaddress(const std::string &newMacaddress);
};

#endif // ROUTER_H
