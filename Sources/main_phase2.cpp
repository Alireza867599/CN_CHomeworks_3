#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>
#include <unordered_set>
#include "datagenerator.h"
#include "packet.h"
#include "tcpheader.h"
#include "datalinkheader.h"
#include "macaddressgenerator.h"
#include "eventcoordinator.h"
#include <QCoreApplication>
#include "ip.h"
#include "port.h"
#include "router.h"
#include "pc.h"
#include <QtConcurrent/QtConcurrent>
#include "topologybuilder.h"
#include "topologycontroller.h"
#include "autonomoussystem.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "simulationhandler.h"
#include "applicationcontext.h"


using namespace std;


 void testNodeConnection(shared_ptr<Node> node1, shared_ptr<Node> node2) {
    bool connected = false;
    for (const auto& port1 : node1->getPorts()) {
        for (const auto& port2 : node2->getPorts()) {
            if (port1->isConnected() && port2->isConnected()) {
                if (port1->getConnectedNode() == node2 && port2->getConnectedNode() == node1) {
                    connected = true;
                    break;
                }
            }
        }
    }
    cout << "Nodes " << node1->getId() << " and " << node2->getId() 
              << (connected ? " are connected." : " are NOT connected.") << endl;
}

void testMeshTopology(TopologyBuilder& builder) {
    builder.buildMeshTopology(3, 2);  
    cout << "Mesh Topology created with 3 routers and 2 PCs." << endl;
    builder.printTopology();  
}

void testStarRingTopology(TopologyBuilder& builder) {
    builder.buildStarRingTopology(4, 2);  
    cout << "Star-Ring Topology created with 4 routers and 2 PCs." << endl;
    builder.printTopology();  
}

void testAutonomousSystem() {
    auto as1 = make_shared<AutonomousSystem>(1);
    auto router1 = make_shared<Router>(1, "192.168.1.1", "00:1A:2B:3C:4D:5E");
    auto pc1 = make_shared<PC>("192.168.0.1", 8080, 1.0, 2.0, 1000.0, 42);
    
    as1->addRouter(router1);
    as1->addPC(pc1);
    as1->setGateway(router1);
    
    cout << "AS1 created with Router ID: " << router1->getId() << " and PC ID: " << pc1->getId() << endl;
    
    auto as2 = make_shared<AutonomousSystem>(2);
    as1->connectToAS(as2);
    
    cout << "AS1 connected to AS2." << endl;
}

void testPacketTransmission() {
    IPv4_t ipv4Address("192.168.0.1");
    TcpHeader tcpHeader;
    DataLinkHeader dataLinkHeader;

    Packet packet(UT::PacketType::Data, &ipv4Address, &tcpHeader, &dataLinkHeader, "Test Payload", 1, 5, 10);

    Port port1, port2;
    QObject::connect(&port1, &Port::packetSent, [](const PacketPtr_t &pkt) {
        cout << "Packet Sent: " << pkt->getPayload() << endl;
    });
    QObject::connect(&port2, &Port::packetReceived, [](const PacketPtr_t &pkt) {
        cout << "Packet Received: " << pkt->getPayload() << endl;
    });

    port1.sendPacket(packet);
    port2.receivePacket(packet);
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    TopologyBuilder builder;

    cout << "Running Mesh Topology Test..." << endl;
    testMeshTopology(builder);

    cout << "Running Star-Ring Topology Test..." << endl;
    testStarRingTopology(builder);

    cout << "Running AutonomousSystem Test..." << endl;
    testAutonomousSystem();

    cout << "Running Packet Transmission Test..." << endl;
    testPacketTransmission();

    return a.exec();
}


