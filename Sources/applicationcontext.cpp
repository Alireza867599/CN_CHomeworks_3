#include "ApplicationContext.h"

ApplicationContext::ApplicationContext(QObject *parent) : QObject(parent)
{
    // Constructor logic, if necessary
}

ApplicationContext::~ApplicationContext()  // Virtual destructor
{
    // Destructor logic, if necessary
}

bool ApplicationContext::loadConfiguration(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file.";
        return false;
    }

    QByteArray fileData = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileData);
    if (doc.isNull()) {
        qDebug() << "Failed to parse JSON.";
        return false;
    }

    QJsonObject json = doc.object();

    // Step 3: Extract basic parameters
    m_simulationDuration = json.value("simulation_duration").toString();
    m_cycleDuration = json.value("cycle_duration").toString();
    m_ttl = json.value("TTL").toInt();
    m_packetsPerSimulation = json.value("packets_per_simulation").toInt();
    m_statisticalDistribution = json.value("statistical_distribution").toString();

    qDebug() << "Simulation Duration:" << m_simulationDuration;
    qDebug() << "Cycle Duration:" << m_cycleDuration;

    // Parse Autonomous Systems
    QJsonArray autonomousSystems = json.value("Autonomous_systems").toArray();
    for (const QJsonValue &asValue : autonomousSystems) {
        QJsonObject asObject = asValue.toObject();

        AutonomousSystem asObj;
        asObj.id = asObject.value("id").toInt();
        asObj.topologyType = asObject.value("topology_type").toString();
        asObj.nodeCount = asObject.value("node_count").toInt();

        // Manually extract the integer lists for asGateways and userGateways
        QJsonArray asGatewaysArray = asObject.value("as_gateways").toArray();
        for (const QJsonValue &value : asGatewaysArray) {
            asObj.asGateways.append(value.toInt());
        }

        QJsonArray userGatewaysArray = asObject.value("user_gateways").toArray();
        for (const QJsonValue &value : userGatewaysArray) {
            asObj.userGateways.append(value.toInt());
        }

        asObj.dhcpServer = asObject.value("dhcp_server").toInt();

        // Manually convert brokenRouters to QList<int>
        QJsonArray brokenRoutersArray = asObject.value("broken_routers").toArray();
        for (const QJsonValue &value : brokenRoutersArray) {
            asObj.brokenRouters.append(value.toInt());
        }

        // Parse gateways
        QJsonArray gateways = asObject.value("gateways").toArray();
        for (const QJsonValue &gatewayValue : gateways) {
            QJsonObject gatewayObject = gatewayValue.toObject();
            Gateway gateway;
            gateway.node = gatewayObject.value("node").toInt();

            // Convert users array to QList<int>
            QJsonArray usersArray = gatewayObject.value("users").toArray();
            for (const QJsonValue &userValue : usersArray) {
                gateway.users.append(userValue.toInt());
            }

            asObj.gateways.append(gateway);
        }

        // Parse connections to other AS
        if (asObject.contains("connect_to_as")) {
            QJsonArray connections = asObject.value("connect_to_as").toArray();
            for (const QJsonValue &connectionValue : connections) {
                QJsonObject connectionObject = connectionValue.toObject();
                ASConnection connection;
                connection.id = connectionObject.value("id").toInt();

                QJsonArray gatewayPairs = connectionObject.value("gateway_pairs").toArray();
                for (const QJsonValue &pairValue : gatewayPairs) {
                    QJsonObject pairObject = pairValue.toObject();
                    int gateway = pairObject.value("gateway").toInt();
                    int connectTo = pairObject.value("connect_to").toInt();
                    connection.gatewayPairs.append(qMakePair(gateway, connectTo));
                }

                asObj.connectionsToAS.append(connection);
            }
        }

        m_autonomousSystems.append(asObj);
    }

    return true;
}

QList<int> ApplicationContext::asGateways(int asId) const
{
    for (const AutonomousSystem &as : m_autonomousSystems) {
        if (as.id == asId) {
            return as.asGateways;
        }
    }
    return QList<int>();
}

QList<int> ApplicationContext::userGateways(int asId) const
{
    for (const AutonomousSystem &as : m_autonomousSystems) {
        if (as.id == asId) {
            return as.userGateways;
        }
    }
    return QList<int>();
}
