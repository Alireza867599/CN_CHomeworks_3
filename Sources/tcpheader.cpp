#include "tcpheader.h"
#include <bitset>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>
TcpHeader::TcpHeader()
    : sourcePort(0), destinationPort(0), sequenceNumber(0), acknowledgmentNumber(0),
    flags(0), windowSize(0), checksum(0), urgentPointer(0) {}


TcpHeader::~TcpHeader() {}

void TcpHeader::setSourcePort(uint16_t port) {
    sourcePort = port;
}

void TcpHeader::setDestinationPort(uint16_t port) {
    destinationPort = port;
}

void TcpHeader::setSequenceNumber(uint32_t sequence) {
    sequenceNumber = sequence;
}

void TcpHeader::setAcknowledgmentNumber(uint32_t ack) {
    acknowledgmentNumber = ack;
}

void TcpHeader::setFlags(bool syn, bool ack, bool fin, bool rst, bool psh, bool urg) {
    flags = (syn << 1) | (ack << 4) | (fin << 0) | (rst << 2) | (psh << 3) | (urg << 5);
}

void TcpHeader::setWindowSize(uint16_t size) {
    windowSize = size;
}

void TcpHeader::setChecksum(uint16_t checksum) {
    this->checksum = checksum;
}

void TcpHeader::setUrgentPointer(uint16_t pointer) {
    urgentPointer = pointer;
}

// Getters
uint16_t TcpHeader::getSourcePort() const {
    return sourcePort;
}

uint16_t TcpHeader::getDestinationPort() const {
    return destinationPort;
}

uint32_t TcpHeader::getSequenceNumber() const {
    return sequenceNumber;
}

uint32_t TcpHeader::getAcknowledgmentNumber() const {
    return acknowledgmentNumber;
}

uint8_t TcpHeader::getFlags() const {
    return flags;
}

uint16_t TcpHeader::getWindowSize() const {
    return windowSize;
}

uint16_t TcpHeader::getChecksum() const {
    return checksum;
}

uint16_t TcpHeader::getUrgentPointer() const {
    return urgentPointer;
}

// Helper methods
std::string TcpHeader::toString() const {
    std::ostringstream oss;
    oss << "TCP Header:\n"
        << "Source Port: " << sourcePort << "\n"
        << "Destination Port: " << destinationPort << "\n"
        << "Sequence Number: " << sequenceNumber << "\n"
        << "Acknowledgment Number: " << acknowledgmentNumber << "\n"
        << "Flags: 0b" << std::bitset<8>(flags) << "\n"
        << "Window Size: " << windowSize << "\n"
        << "Checksum: 0x" << std::hex << std::setw(4) << std::setfill('0') << checksum << "\n"
        << "Urgent Pointer: " << urgentPointer << "\n";
    return std::string(oss.str());
}

std::vector<uint8_t> TcpHeader::serialize() const {
    std::vector<uint8_t> data(20); // TCP header is 20 bytes without options
    data[0] = sourcePort >> 8;     // Source Port (High Byte)
    data[1] = sourcePort & 0xFF;   // Source Port (Low Byte)
    data[2] = destinationPort >> 8;
    data[3] = destinationPort & 0xFF;
    data[4] = (sequenceNumber >> 24) & 0xFF;
    data[5] = (sequenceNumber >> 16) & 0xFF;
    data[6] = (sequenceNumber >> 8) & 0xFF;
    data[7] = sequenceNumber & 0xFF;
    data[8] = (acknowledgmentNumber >> 24) & 0xFF;
    data[9] = (acknowledgmentNumber >> 16) & 0xFF;
    data[10] = (acknowledgmentNumber >> 8) & 0xFF;
    data[11] = acknowledgmentNumber & 0xFF;
    data[12] = (5 << 4);           // Header length (5 words, no options)
    data[13] = flags;             // Flags
    data[14] = windowSize >> 8;
    data[15] = windowSize & 0xFF;
    data[16] = checksum >> 8;
    data[17] = checksum & 0xFF;
    data[18] = urgentPointer >> 8;
    data[19] = urgentPointer & 0xFF;
    return data;
}

void TcpHeader::deserialize(const std::vector<uint8_t>& data) {
    if (data.size() < 20) {
        throw std::invalid_argument("Data too short for TCP header");
    }
    sourcePort = (data[0] << 8) | data[1];
    destinationPort = (data[2] << 8) | data[3];
    sequenceNumber = (data[4] << 24) | (data[5] << 16) | (data[6] << 8) | data[7];
    acknowledgmentNumber = (data[8] << 24) | (data[9] << 16) | (data[10] << 8) | data[11];
    flags = data[13];
    windowSize = (data[14] << 8) | data[15];
    checksum = (data[16] << 8) | data[17];
    urgentPointer = (data[18] << 8) | data[19];
}
