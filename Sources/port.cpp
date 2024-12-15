#include "port.h"
#include <QDebug>

Port::Port(QObject *parent)
    : QObject(parent),
    m_number(0),
    m_numberOfPacketsSent(0),
    m_routerIP("127.0.0.1") // Default value; can be modified as needed
{
}

Port::~Port() {}

void Port::sendPacket(const PacketPtr_t &data)
{
    if (!data) {
        qWarning("sendPacket: Attempting to send a null packet.");
        return;
    }

    // Increment packet counter
    ++m_numberOfPacketsSent;

    // Emit the packetSent signal
    emit packetSent(data);

    qDebug() << "Packet sent. Total sent packets:" << m_numberOfPacketsSent;
}

void Port::receivePacket(const PacketPtr_t &data)
{
    if (!data) {
        qWarning("receivePacket: Received a null packet.");
        return;
    }

    // Emit the packetReceived signal
    emit packetReceived(data);

    qDebug() << "Packet received.";
}

QString Port::routerIP() const
{
    return m_routerIP;
}

void Port::setRouterIP(const QString &newRouterIP)
{
    m_routerIP = newRouterIP;
}

uint64_t Port::numberOfPacketsSent() const
{
    return m_numberOfPacketsSent;
}

void Port::setNumberOfPacketsSent(uint64_t newNumberOfPacketsSent)
{
    m_numberOfPacketsSent = newNumberOfPacketsSent;
}

uint8_t Port::number() const
{
    return m_number;
}

void Port::setNumber(uint8_t newNumber)
{
    m_number = newNumber;
}
