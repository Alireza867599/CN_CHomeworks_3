#ifndef APPLICATIONCONTEXT_H
#define APPLICATIONCONTEXT_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QDebug>
#include <QList>

class ApplicationContext : public QObject
{
    Q_OBJECT

public:
    explicit ApplicationContext(QObject *parent = nullptr);
    ~ApplicationContext();

    bool loadConfiguration(const QString &filePath);

    // Getter methods to access the parsed configuration values
    QString simulationDuration() const { return m_simulationDuration; }
    QString cycleDuration() const { return m_cycleDuration; }
    int ttl() const { return m_ttl; }
    int packetsPerSimulation() const { return m_packetsPerSimulation; }
    QString statisticalDistribution() const { return m_statisticalDistribution; }

    // Autonomous systems related methods
    QList<int> asGateways(int asId) const;
    QList<int> userGateways(int asId) const;

private:
    QString m_simulationDuration;
    QString m_cycleDuration;
    int m_ttl;
    int m_packetsPerSimulation;
    QString m_statisticalDistribution;

    struct Gateway {
        int node;
        QList<int> users;
    };

    struct ASConnection {
        int id;
        QList<QPair<int, int>> gatewayPairs;
    };

    struct AutonomousSystem {
        int id;
        QString topologyType;
        int nodeCount;
        QList<int> asGateways;
        QList<int> userGateways;
        int dhcpServer;
        QList<int> brokenRouters;
        QList<Gateway> gateways;
        QList<ASConnection> connectionsToAS;
    };

    QList<AutonomousSystem> m_autonomousSystems;
};

#endif // APPLICATIONCONTEXT_H
