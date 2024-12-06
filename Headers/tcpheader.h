#ifndef TCPHEADER_H
#define TCPHEADER_H
#include <cstdint>
#include <QString>
#include <vector>
#include <string>
class TcpHeader
{

public:
    TcpHeader();               // Constructor
    ~TcpHeader();              // Destructor

    // Setters
    void setSourcePort(uint16_t port);
    void setDestinationPort(uint16_t port);
    void setSequenceNumber(uint32_t sequence);
    void setAcknowledgmentNumber(uint32_t ack);
    void setFlags(bool syn, bool ack, bool fin, bool rst, bool psh, bool urg);
    void setWindowSize(uint16_t size);
    void setChecksum(uint16_t checksum);
    void setUrgentPointer(uint16_t pointer);

    // Getters
    uint16_t getSourcePort() const;
    uint16_t getDestinationPort() const;
    uint32_t getSequenceNumber() const;
    uint32_t getAcknowledgmentNumber() const;
    uint8_t getFlags() const;  // Combined flag byte
    uint16_t getWindowSize() const;
    uint16_t getChecksum() const;
    uint16_t getUrgentPointer() const;

    // Helper methods
    std::string toString() const;  // Converts header to a readable string
    std::vector<uint8_t> serialize() const; // Serialize the header for transmission
    void deserialize(const std::vector<uint8_t>& data); // Deserialize data into the header

private:
    uint16_t sourcePort;
    uint16_t destinationPort;
    uint32_t sequenceNumber;
    uint32_t acknowledgmentNumber;
    uint8_t flags;             // Combined 8-bit flags (SYN, ACK, FIN, etc.)
    uint16_t windowSize;
    uint16_t checksum;
    uint16_t urgentPointer;
};

#endif // TCPHEADER_H
