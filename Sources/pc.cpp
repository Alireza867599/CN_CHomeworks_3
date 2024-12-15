#include "pc.h"
#include <QString>
#include "macaddressgenerator.h"
#include <QThread>
GenerateMACAddress macaddress;

PC::PC(const std::string& ip, int port, double shape, double scale, double rate, unsigned int seed)
    : Node(macaddress.generateUniqueMAC()),
    dataGenerator(shape, scale, rate, seed),
    ipAddress(ip),
    port(port) {}

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

bool PC::connectToRouter(Router* router) {
    return router->connectUser(port, this);
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
