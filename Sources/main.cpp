#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>
#include <unordered_set>
#include "datagenerator.h"
#include "macaddressgenerator.h"
using namespace std;


#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>


int main(int argc, char *argv[])
{
        QCoreApplication a(argc, argv);

        // Step 1: Open and read the JSON file
        QFile file("C:/Users/Entezar/Documents/CA3/config.js");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Failed to open the file.";
            return -1;
        }

        QByteArray fileData = file.readAll();
        file.close();

        // Step 2: Parse the JSON content
        QJsonDocument doc = QJsonDocument::fromJson(fileData);
        if (doc.isNull()) {
            qDebug() << "Failed to parse JSON.";
            return -1;
        }

        QJsonObject json = doc.object();

        // Step 3: Extract basic parameters
        QString simulationDuration = json.value("simulation_duration").toString();
        QString cycleDuration = json.value("cycle_duration").toString();
        int ttl = json.value("TTL").toInt();
        int packetsPerSimulation = json.value("packets_per_simulation").toInt();
        QString statisticalDistribution = json.value("statistical_distribution").toString();
        int routerBufferSize = json.value("router_buffer_size").toInt();
        int routerPortCount = json.value("router_port_count").toInt();
        QString routingProtocol = json.value("routing_protocol").toString();
        QString routingTableUpdateInterval = json.value("routing_table_update_interval").toString();
        bool routingPerPort = json.value("routing_per_port").toBool();
        int routingTableSize = json.value("routing_table_size").toInt();
        int routingPacketsPerPortCycle = json.value("routing_packets_per_port_cycle").toInt();

        qDebug() << "Simulation Duration:" << simulationDuration;
        qDebug() << "Cycle Duration:" << cycleDuration;

        // Step 4: Parse Autonomous Systems
        QJsonArray autonomousSystems = json.value("Autonomous_systems").toArray();

        for (const QJsonValue &asValue : autonomousSystems) {
            QJsonObject asObject = asValue.toObject();

            int asId = asObject.value("id").toInt();
            QString topologyType = asObject.value("topology_type").toString();
            int nodeCount = asObject.value("node_count").toInt();
            QJsonArray asGateways = asObject.value("as_gateways").toArray();
            QJsonArray userGateways = asObject.value("user_gateways").toArray();
            int dhcpServer = asObject.value("dhcp_server").toInt();
            QJsonArray brokenRouters = asObject.value("broken_routers").toArray();

            qDebug() << "AS ID:" << asId;
            qDebug() << "Topology Type:" << topologyType;
            qDebug() << "Node Count:" << nodeCount;
            qDebug() << "AS Gateways:" << asGateways;
            qDebug() << "User Gateways:" << userGateways;
            qDebug() << "DHCP Server:" << dhcpServer;

            qDebug() << "ttl:" << ttl;
            qDebug() << "routerBufferSize:" << routerBufferSize;
            qDebug() << "packetsPerSimulation :" << packetsPerSimulation;
            qDebug() << "routerPortCount :" << routerPortCount;
            qDebug() << "routingPerPort :" << routingPerPort;
            qDebug() << "routingTableSize :" << routingTableSize;
            qDebug() << "routingPacketsPerPortCycle :" << routingPacketsPerPortCycle;



            // Parse gateways
            QJsonArray gateways = asObject.value("gateways").toArray();
            for (const QJsonValue &gatewayValue : gateways) {
                QJsonObject gatewayObject = gatewayValue.toObject();
                int node = gatewayObject.value("node").toInt();
                QJsonArray users = gatewayObject.value("users").toArray();
                qDebug() << "Gateway Node:" << node;
                qDebug() << "Users:" << users;
            }

            // Parse connections to other AS
            if (asObject.contains("connect_to_as")) {
                QJsonArray connections = asObject.value("connect_to_as").toArray();
                for (const QJsonValue &connectionValue : connections) {
                    QJsonObject connectionObject = connectionValue.toObject();
                    int connectToId = connectionObject.value("id").toInt();
                    QJsonArray gatewayPairs = connectionObject.value("gateway_pairs").toArray();
                    qDebug() << "Connect to AS ID:" << connectToId;

                    for (const QJsonValue &pairValue : gatewayPairs) {
                        QJsonObject pairObject = pairValue.toObject();
                        int gateway = pairObject.value("gateway").toInt();
                        int connectTo = pairObject.value("connect_to").toInt();
                        qDebug() << "Gateway:" << gateway << "Connect to:" << connectTo;
                    }
                }
            }
        }

        GenerateMACAddress macGenerator;

        // Generate and display 5 unique MAC addresses
        for (int i = 0; i < 10; ++i) {
            string mac = macGenerator.generateUniqueMAC();
            cout << "Generated MAC Address"<<i<<"\t"<< mac << endl;
        }

        double shape = 2.0;  // Shape parameter (alpha)
        double scale = 1.0;  // Scale parameter (xm)
        double rate = 10.0;  // Events per second

        DataGenerator generator(shape, scale, rate);

        double time_interval = 2.0;  // Time in seconds
        std::vector<double> data = generator.generateOverTime(time_interval);

        std::cout << "Generated Data Over " << time_interval << " Seconds: ";
        for (double num : data) {
            std::cout << num << " ";
        }
        std::cout << std::endl;

        return a.exec();
    }



