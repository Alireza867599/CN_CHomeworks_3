#ifndef PACKET_H
#define PACKET_H
#include <iostream>
using namespace std;

#include "datalinkheader.h"
#include "tcpheader.h"
#include "ip.h"
#include "globals.h"
#include <vector>
#include <string>






class Packet
{
private:
    UT::PacketType packetType;
    UT::PacketControlType controlType;
    bool isControlPacket;
    IP<UT::IPVersion::IPv4>* ipHeader;
    TcpHeader* tcpHeader;
    DataLinkHeader* dataLinkHeader;
    std::string payload;
    int sequenceNumber;
    int waitingCycles;
    int totalCycles;
    std::vector<std::string> path;
    bool dropped;

public:
    Packet(UT::PacketType type, IP<UT::IPVersion::IPv4>* ip, TcpHeader* tcp, DataLinkHeader* dataLink, const std::string& dataPayload, int seqNum, int waitCycles, int total);


    Packet(UT::PacketControlType controlType ,  IP<UT::IPVersion::IPv4>* ipHeader, DataLinkHeader* dataLink,
           int seqNum, int waitCycles, int total)
        : packetType(UT::PacketType::Control),controlType(controlType),ipHeader(ipHeader),
        tcpHeader(nullptr), dataLinkHeader(dataLink), sequenceNumber(seqNum),
        waitingCycles(waitCycles), totalCycles(total) {}

    ~Packet();

    // Getters and Setters
    UT::PacketType getPacketType() const;
    UT::PacketControlType getControlType() const;
    TcpHeader* getTCPHeader() const;
    DataLinkHeader* getDataLinkHeader() const;
    const std::string& getPayload() const;
    int getSequenceNumber() const;
    int getWaitingCycles() const;
    int getTotalCycles() const;
    const std::vector<std::string>& getPath() const;

    void setPath(const std::vector<std::string>& hops);
    void addHop(const std::string& hop);
    IP<UT::IPVersion::IPv4> *getIpHeader() const;
    void setIpHeader(IP<UT::IPVersion::IPv4> *newIpHeader);
    bool getIsControlPacket() const;
    void setIsControlPacket(bool newIsControlPacket);
    bool getDropped() const;
    void setDropped(bool newDropped);
};

#endif // PACKET_H
