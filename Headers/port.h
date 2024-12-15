// port.h
#ifndef PORT_H
#define PORT_H

#include "packet.h"

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <cstdint>

typedef QSharedPointer<Packet> PacketPtr_t;

class Port : public QObject
{
    Q_OBJECT

public:
    explicit Port(QObject *parent = nullptr);
    ~Port() override;

    uint8_t number() const;
    void setNumber(uint8_t newNumber);

    uint64_t numberOfPacketsSent() const;
    void setNumberOfPacketsSent(uint64_t newNumberOfPacketsSent);

    QString routerIP() const;
    void setRouterIP(const QString &newRouterIP);

Q_SIGNALS:
    void packetSent(const PacketPtr_t &data);
    void packetReceived(const PacketPtr_t &data);

public Q_SLOTS:
    void sendPacket(const PacketPtr_t &data);
    void receivePacket(const PacketPtr_t &data);

private:
    uint8_t  m_number;                  // Unique port number
    uint64_t m_numberOfPacketsSent;     // Count of packets sent
    QString  m_routerIP;                // IP address of the associated router
};

typedef QSharedPointer<Port> PortPtr_t;

#endif    // PORT_H
