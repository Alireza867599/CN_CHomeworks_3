#include "router.h"
#include <algorithm>
#include "dhcpserver.h"


bool Router::getIsDhcpEnabled() const
{
    return isDhcpEnabled;
}

void Router::setIsDhcpEnabled(bool newIsDhcpEnabled)
{
    isDhcpEnabled = newIsDhcpEnabled;
}

std::string Router::getMacaddress() const
{
    return macaddress;
}

void Router::setMacaddress(const std::string &newMacaddress)
{
    macaddress = newMacaddress;
}

Router::Router(int id , const std::string& ipAddress,const std::string& macaddress, int asid)
    : ipAddress(ipAddress) ,routerId(id) , asId(asid) , macaddress(macaddress)  {}

int Router::getId() const {
    return routerId;
}

int Router::getAsId() const
{
    return asId;
}

void Router::addRoutingTableEntry(const RoutingTableEntry &entry) {
    routingTable.push_back(entry);
}

void Router::removeRoutingTableEntry(const std::string &destinationNetwork) {
    routingTable.erase(std::remove_if(routingTable.begin(), routingTable.end(),
                                      [&](const RoutingTableEntry &entry) {
                                          return entry.destinationNetwork == destinationNetwork;
                                      }),
                       routingTable.end());
}

void Router::displayRoutingTable() const {
    std::cout << "Routing Table for Router ID: " << routerId << "\n";
    for (const auto &entry : routingTable) {
        std::cout << "Dest: " << entry.destinationNetwork
                  << ", Subnet: " << entry.subnetMask
                  << ", Gateway: " << entry.gateway
                  << ", Metric: " << entry.metric
                  << ", Protocol: " << entry.protocol << "\n";
    }
}

RoutingTableEntry Router::findBestRoute(const std::string &destination) const {
    for (const auto &entry : routingTable) {
        if (destination.find(entry.destinationNetwork) != std::string::npos) {
            return entry;
        }
    }
    throw std::runtime_error("Route not found");
}

void Router::setBufferPolicy(BufferManagementPolicy::Method method) {
    bufferPolicy.method = method;
}

void Router::sendPacketToBuffer(const Packet &packet) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    switch (bufferPolicy.method) {
    case BufferManagementPolicy::FIFO:
        if (fifoBuffer.size() < BUFFER_CAPACITY) {
            fifoBuffer.push(packet);
        }
        break;
    case BufferManagementPolicy::LIFO:
        if (lifoBuffer.size() < BUFFER_CAPACITY) {
            lifoBuffer.push(packet);
        }
        break;
    case BufferManagementPolicy::RED:
        if (fifoBuffer.size() >= BUFFER_CAPACITY / 2 && rand() % 2 == 0) {
            std::cout << "RED policy: dropping packet\n";
        } else {
            fifoBuffer.push(packet);
        }
        break;
    }
}

Packet Router::retrievePacketFromBuffer() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    if (bufferPolicy.method == BufferManagementPolicy::FIFO && !fifoBuffer.empty()) {
        Packet packet = fifoBuffer.front();
        fifoBuffer.pop();
        return packet;
    } else if (bufferPolicy.method == BufferManagementPolicy::LIFO && !lifoBuffer.empty()) {
        Packet packet = lifoBuffer.top();
        lifoBuffer.pop();
        return packet;
    }
    throw std::runtime_error("Buffer is empty");
}

bool Router::connectRouter(int routerId, std::shared_ptr<Router> router) {
    for (const auto& connectedRouter : connectedRouters) {
        if (connectedRouter->getId() == routerId) {
            return false; // Already connected
        }
    }
    connectedRouters.push_back(router);
    return true;
}
// void Router::enableDhcp(const std::string& ipRange, int asId) {
//     this->isDhcpEnabled = true;
//     this->dhcpIpRange = ipRange;
//     this->dhcpAsId = asId;
//     std::cout << "DHCP enabled on Router ID " << id
//               << " with IP range " << ipRange
//               << " for AS " << asId << "\n";
// }
std::vector<std::shared_ptr<Router>> Router::getConnectedRouters() const {
    return connectedRouters;
}
void Router::connectUser(int pcId, std::shared_ptr<PC> pc) {
    connectedPCs.push_back(pc);
    std::cout << "PC with ID " << pcId << " connected to Router " << getId() << "\n";
}

const std::vector<std::shared_ptr<PC> > &Router::getConnectedPCs() const
{
    return connectedPCs;
}

std::vector<std::shared_ptr<Port>>Router::getPorts() const {
    return m_ports;
}
void Router::addPort(std::shared_ptr<Port> port) {
    m_ports.push_back(port);
}
// Send a packet from this router
void Router::sendPacket(const Packet& packet) {
    std::cout << "Router " << id << " sent packet with sequence number "
              << packet.getSequenceNumber()
              << " and payload: " << packet.getPayload()
              << "\n";
}
void Router::enableDhcp(const string& ipRange, int asID) {
    dhcpServer = std::make_unique<DHCP>(ipRange, asID);
    isDhcpEnabled = true;
}


std::string Router::processDhcpDiscover(int nodeId, const std::string& macAddress) {
    if (!isDhcpEnabled) {
        std::cout << "Router " << id << " is not DHCP-enabled.\n";
        return "";
    }

    std::string assignedIP = dhcpServer->assignIP(nodeId, macAddress);
    if (!assignedIP.empty()) {
        std::cout << "Router " << id << " assigned IP: " << assignedIP
                  << " to Node " << nodeId << " via DHCP.\n";
    } else {
        std::cout << "Router " << id << " failed to assign IP to Node " << nodeId << "\n";
    }

    return assignedIP;
}
void Router::saveDhcpAllocations() {
    if (isDhcpEnabled && dhcpServer) {
        dhcpServer->saveAllocationsToFile();
    }
}



void Router::requestIP(shared_ptr<Router> router) {
    if (!ipAddress.empty()) {
        cout << "Node " << routerId << " already has IP: " << ipAddress << "\n";
        return;
    }

    string assignedIP = router->processDhcpDiscover(routerId, macAddress);
    if (!assignedIP.empty()) {
        setIPAddress(assignedIP);  // Ensure the IP is set correctly
        cout << "Node " << routerId << " assigned IP: " << assignedIP << " via DHCP.\n";
    } else {
        cout << "Failed to assign IP to Node " << routerId << "\n";
    }
}

void Router::handleIPRequest(std::shared_ptr<Node> node) {
    if (!node->getIPAddress().empty()) {
        std::cout << "Node " << node->getId() << " already has IP: " << node->getIPAddress() << "\n";
        return;
    }

    std::string assignedIP = processDhcpDiscover(node->getId(), node->getMacAddress());
    if (!assignedIP.empty()) {
        node->setIPAddress(assignedIP);
        std::cout << "Node " << node->getId() << " assigned IP: " << assignedIP << " via DHCP.\n";
    } else {
        std::cout << "Failed to assign IP to Node " << node->getId() << "\n";
    }
}
