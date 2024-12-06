#include "packet.h"


Packet::Packet(UT::PacketType type, /*IPHeader *ip,*/ TcpHeader *tcp, DataLinkHeader *dataLink, const string &dataPayload, int seqNum, int waitCycles, int total)
    : packetType(type), /*ipHeader(ip),*/ tcpHeader(tcp), /*bgpHeader(nullptr),*/
    dataLinkHeader(dataLink), payload(dataPayload), sequenceNumber(seqNum),
    waitingCycles(waitCycles), totalCycles(total) {}

Packet::~Packet()
{
    // delete ipHeader;
    delete tcpHeader;
    // delete bgpHeader;
    delete dataLinkHeader;
}

UT::PacketType Packet::getPacketType() const { return packetType; }

UT::PacketControlType Packet::getControlType() const { return controlType; }

// IPHeader *Packet::getIPHeader() const { return ipHeader; }

TcpHeader *Packet::getTCPHeader() const { return tcpHeader; }

// BGPHeader *Packet::getBGPHeader() const { return bgpHeader; }

DataLinkHeader *Packet::getDataLinkHeader() const { return dataLinkHeader; }

const string &Packet::getPayload() const { return payload; }

int Packet::getSequenceNumber() const { return sequenceNumber; }

int Packet::getWaitingCycles() const { return waitingCycles; }

int Packet::getTotalCycles() const { return totalCycles; }

const std::vector<string> &Packet::getPath() const { return path; }

void Packet::setPath(const std::vector<string> &hops) {
    path = hops;
}

void Packet::addHop(const std::string& hop) {
    path.push_back(hop);
}


