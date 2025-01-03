#include "pc.h"
#include <QString>
#include "macaddressgenerator.h"
#include <QThread>
GenerateMACAddress macaddress;


std::string PC::getIpAddress() const
{
    return ipAddress;
}

std::string PC::getMacaddress() const
{
    return macaddress;
}

void PC::setMacaddress(const std::string &newMacaddress)
{
    macaddress = newMacaddress;
}

PC::PC() : dataGenerator(42,1.1, 1.9, 10), PCID(0) { // Provide a valid argument for DataGenerator
    // Default initialization
}
// Another constructor example with parameters
PC::PC(int id ) : dataGenerator(42,1.1, 1.9, 10), PCID(id) { // Example initialization
    // Custom initialization
}
void PC::setOtherIPs(const std::vector<std::string>& ips) {
    otherIPs = ips;
}

void PC::generateAndSendPackets(int numPackets, const std::string& targetIP, Router* gateway) {
    for (int i = 0; i < numPackets; ++i) {
        // Generate payload data
        double payloadData = dataGenerator.generate();

        // Convert std::string to QString
        QString srcIP = QString::fromStdString(ipAddress);
        QString destIP = QString::fromStdString(targetIP);

        // Create packet
        auto packet = std::make_unique<Packet>(
            UT::PacketType::Data, // Packet type
            new IP<UT::IPVersion::IPv4>(srcIP), // IP header
            nullptr, // TCP header (not specified)
            nullptr, // Data link header (not specified)
            std::to_string(payloadData), // Payload
            i + 1, // Sequence number
            0,     // Waiting cycles
            0      // Total cycles
            );

        // Log sent packet (thread-safe)
        {
            QMutexLocker locker(&mutex); // Lock mutex
            sentPackets[i + 1] = std::move(packet);
        }

        // Send packet through the gateway
        gateway->sendPacket(*sentPackets[i + 1]);
    }
}



void PC::processIncomingPacket(const Packet& packet) {
    QMutexLocker locker(&mutex); // Lock mutex for thread-safe output
    std::cout << "PC with IP: " << ipAddress
              << " received packet with payload: " << packet.getPayload() << "\n";
}

void PC::logPacket(const Packet& packet) {
    QMutexLocker locker(&mutex); // Lock mutex for thread-safe access
    std::cout << "Logging packet with sequence number: " << packet.getSequenceNumber() << "\n";
}

void PC::retransmitPacket(int sequenceNumber, Router* gateway) {
    QMutexLocker locker(&mutex); // Lock mutex for thread-safe access
    if (sentPackets.find(sequenceNumber) != sentPackets.end()) {
        gateway->sendPacket(*sentPackets[sequenceNumber]);
    } else {
        std::cout << "Packet with sequence number " << sequenceNumber << " not found for retransmission.\n";
    }
}

void PC::addPort(std::shared_ptr<Port> port) {
    m_ports.push_back(port);
}

std::vector<std::shared_ptr<Port>> PC::getPorts() const {
    return m_ports; // Return the list of ports
}
bool PC::connectToRouter(Router* router) {
    if (!router) {
        return false;
    }
    std::cout << "PC " << PCID << " connecting to router " << router->getId() << std::endl;
    return true;
}
void PC::requestIP(std::shared_ptr<Router> dhcpServer) {
    if (!ipAddress.empty()) {
        std::cout << "PC " << PCID << " already has IP: " << ipAddress << "\n";
        return;
    }

    std::string assignedIP = dhcpServer->processDhcpDiscover(PCID, macAddress);
    if (!assignedIP.empty()) {
        setIPAddress(assignedIP);  // Ensure the IP is set correctly
        std::cout << "PC " << PCID << " assigned IP: " << assignedIP << " via DHCP.\n";
    } else {
        std::cout << "Failed to assign IP to PC " << PCID << "\n";
    }
}
