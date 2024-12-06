#ifndef PACKET_H
#define PACKET_H
#include <iostream>
using namespace std;


#include "globals.h"
#include <vector>
#include <string>

class IPHeader
{
public:
    UT::IPVersion ipVersion;
    std::string sourceIP;
    std::string destinationIP;

    IPHeader(UT::IPVersion version, const std::string& src, const std::string& dest)
        : ipVersion(version), sourceIP(src), destinationIP(dest) {}
};

class TCPHeader
{
public:
    int sourcePort;
    int destinationPort;

    TCPHeader(int srcPort, int destPort) : sourcePort(srcPort), destinationPort(destPort) {}
};

class BGPHeader
{
public:
    std::string bgpInfo;

    BGPHeader(const std::string& info) : bgpInfo(info) {}
};

class DataLinkHeader
{
public:
    std::string macSource;
    std::string macDestination;

    DataLinkHeader(const std::string& src, const std::string& dest)
        : macSource(src), macDestination(dest) {}
};

class Packet
{
private:
    UT::PacketType packetType;
    UT::PacketControlType controlType;
    IPHeader* ipHeader;
    TCPHeader* tcpHeader;
    BGPHeader* bgpHeader;
    DataLinkHeader* dataLinkHeader;
    std::string payload;
    int sequenceNumber;
    int waitingCycles;
    int totalCycles;
    std::vector<std::string> path;

public:
    Packet(UT::PacketType type, IPHeader* ip, TCPHeader* tcp, DataLinkHeader* dataLink,
           const std::string& dataPayload, int seqNum, int waitCycles, int total);

    // For control packets with BGP Header
    Packet(UT::PacketControlType controlType, IPHeader* ip, BGPHeader* bgp, DataLinkHeader* dataLink,
           int seqNum, int waitCycles, int total)
        : packetType(UT::PacketType::Control), controlType(controlType), ipHeader(ip),
        tcpHeader(nullptr), bgpHeader(bgp), dataLinkHeader(dataLink), sequenceNumber(seqNum),
        waitingCycles(waitCycles), totalCycles(total) {}

    ~Packet();

    // Getters and Setters
    UT::PacketType getPacketType() const;
    UT::PacketControlType getControlType() const;
    IPHeader* getIPHeader() const;
    TCPHeader* getTCPHeader() const;
    BGPHeader* getBGPHeader() const;
    DataLinkHeader* getDataLinkHeader() const;
    const std::string& getPayload() const;
    int getSequenceNumber() const;
    int getWaitingCycles() const;
    int getTotalCycles() const;
    const std::vector<std::string>& getPath() const;

    void setPath(const std::vector<std::string>& hops);
    void addHop(const std::string& hop);
};

#endif // PACKET_H
