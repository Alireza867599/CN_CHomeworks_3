#include "packet.h"


IP<UT::IPVersion::IPv4> *Packet::getIpHeader() const
{
    return ipHeader;
}

void Packet::setIpHeader(IP<UT::IPVersion::IPv4> *newIpHeader)
{
    ipHeader = newIpHeader;
}
bool Packet::getIsControlPacket() const
{
    return isControlPacket;
}

void Packet::setIsControlPacket(bool newIsControlPacket)
{
    isControlPacket = newIsControlPacket;
}

bool Packet::getDropped() const
{
    return dropped;
}

void Packet::setDropped(bool newDropped)
{
    dropped = newDropped;
}

Packet::Packet(UT::PacketType type, IP<UT::IPVersion::IPv4>* ip, TcpHeader* tcp, DataLinkHeader* dataLink, const std::string& dataPayload, int seqNum, int waitCycles, int total)
    : packetType(type), controlType(UT::PacketControlType::DHCPRequest), ipHeader(ip),
    tcpHeader(tcp), dataLinkHeader(dataLink), payload(dataPayload),
    sequenceNumber(seqNum), waitingCycles(waitCycles), totalCycles(total) {}

Packet::~Packet()
{
    delete ipHeader;
    delete tcpHeader;
    delete dataLinkHeader;
}

UT::PacketType Packet::getPacketType() const { return packetType; }

UT::PacketControlType Packet::getControlType() const { return controlType; }


TcpHeader *Packet::getTCPHeader() const { return tcpHeader; }


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


