#include "router.h"

IPv4_t ipv4Address(QString("192.168.2.1"));

TcpHeader* tcpHeader = new TcpHeader();
DataLinkHeader* dataLinkHeader = new DataLinkHeader();

std::string payload = "Example payload";
int sequenceNumber = 1;
int waitingCycles = 10;
int totalCycles = 100;




Router::Router(int id, const std::string &ipAddress, const std::string &macAddress)
    : Node(macAddress), id(id), ipAddress(ipAddress), isDhcpEnabled(false), isGatewayAS(false), isGatewayUser(false) {}

// Routing Table Methods
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
    std::cout << "Routing Table for Router ID: " << id << "\n";
    for (const auto &entry : routingTable) {
        std::cout << "Dest: " << entry.destinationNetwork
                  << ", Subnet: " << entry.subnetMask
                  << ", Gateway: " << entry.gateway
                  << ", Metric: " << entry.metric
                  << ", Protocol: " << entry.protocol << "\n";
    }
}

RoutingTableEntry Router::findBestRoute(const std::string &destination) const {
    // Simplified logic to find a route
    for (const auto &entry : routingTable) {
        if (destination.find(entry.destinationNetwork) != std::string::npos) {
            return entry;
        }
    }
    throw std::runtime_error("Route not found");
}

// Buffer Management Methods
void Router::setBufferPolicy(BufferManagementPolicy::Method method) {
    bufferPolicy.method = method;
}

void Router::sendPacketToBuffer(const Packet &packet) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    switch (bufferPolicy.method) {
    case BufferManagementPolicy::FIFO:
        if (fifoBuffer.size() < BUFFER_CAPACITY) {
            fifoBuffer.push(packet);
        } else {
            std::cout << "FIFO Buffer full, dropping packet: " << packet.getPayload() << "\n";
        }
        break;
    case BufferManagementPolicy::LIFO:
        if (lifoBuffer.size() < BUFFER_CAPACITY) {
            lifoBuffer.push(packet);
        } else {
            std::cout << "LIFO Buffer full, dropping packet: " << packet.getPayload() << "\n";
        }
        break;
    case BufferManagementPolicy::RED:
        // Simplified RED logic
        if (fifoBuffer.size() >= BUFFER_CAPACITY / 2 && rand() % 2 == 0) {
            std::cout << "RED policy: dropping packet: " << packet.getPayload()<< "\n";
        } else {
            fifoBuffer.push(packet);
        }
        break;
    }
}


Packet Router::retrievePacketFromBuffer() {
    std::lock_guard<std::mutex> lock(bufferMutex);
    Packet packet(UT::PacketType::Control, &ipv4Address, tcpHeader, dataLinkHeader, "Control Packet", 0, 0, 1);
    switch (bufferPolicy.method) {
    case BufferManagementPolicy::FIFO:
        if (!fifoBuffer.empty()) {
            packet = fifoBuffer.front();
            fifoBuffer.pop();
        }
        break;
    case BufferManagementPolicy::LIFO:
        if (!lifoBuffer.empty()) {
            packet = lifoBuffer.top();
            lifoBuffer.pop();
        }
        break;
    default:
        break;
    }
    return packet;
}

// Existing methods
void Router::enableDhcp() {
    isDhcpEnabled = true;
}

void Router::setAsGatewayAS() {
    isGatewayAS = true;
}

void Router::setAsGatewayUser() {
    isGatewayUser = true;
}

bool Router::connectRouter(int port, Router *router) {
    if (port >= 1 && port <= PORT_COUNT && connectedRouters.find(port) == connectedRouters.end()) {
        connectedRouters[port] = router;
        return true;
    }
    return false;
}

bool Router::connectUser(int port, Node *user) {
    if (port >= 1 && port <= PORT_COUNT) {
        connectedUsers[port].push_back(user);
        return true;
    }
    return false;
}

void Router::processBuffer() {
    while (true) {
        Packet packet = retrievePacketFromBuffer();
        if (!packet.getPayload().empty()) {
            std::cout << "Processing packet: " << packet.getPayload() << "\n";
        }
    }
}
void Router::sendPacket(const Packet& packet) {
    std::lock_guard<std::mutex> lock(bufferMutex);
    if (buffer.size() < BUFFER_CAPACITY) {
        buffer.push(packet);
    } else {
        // Buffer is full, handle packet dropping
        if (packet.getIsControlPacket()) {
            Packet droppedPacket = buffer.back();
            buffer.pop();
            droppedPacket.setDropped(true);
            buffer.push(packet);
        } else {
            // Drop the packet
            Packet droppedPacket = packet;
            droppedPacket.setDropped(true);
            std::cout << "Packet dropped: " << droppedPacket.getPayload() << std::endl;
        }
    }
}


int Router::getId() const {
    return id;
}

std::string Router::getIpAddress() const {
    return ipAddress;
}
