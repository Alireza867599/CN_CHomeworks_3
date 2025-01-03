#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <string>
#include <unordered_set>
#include "datagenerator.h"
#include "dhcpserver.h"
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
#include "autonomoussystem.h"
#include <QtConcurrent/QtConcurrent>
// #include "topolgybuilder.h"
// #include "topologycontroler.h"
// #include "autonomoussystem.h"
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include "simulationhandler.h"
// #include "applicationcontext.h"
#include "topolgybuilder.h"
#include "rip.h"
using namespace std;






// int main() {
//     // Create an application context and load configuration
//     ApplicationContext appContext;
//     if (!appContext.loadConfiguration("C:/Users/Entezar/Documents/CA3/config.js")) {
//         std::cerr << "Failed to load configuration.\n";
//         return -1;
//     }

//     // Iterate over autonomous systems to set up DHCP servers
//     for (const AutonomousSystem& as : appContext.getAutonomousSystems()) {
//         std::unordered_map<int, std::string> nodeIPs;
//         for (const Gateway& gateway : as.gateways) {
//             int nodeId = gateway.node;
//             QStringList ipRange = appContext.getUserIps(as.id, nodeId);
//             for (const QString& ip : ipRange) {
//                 nodeIPs[nodeId] = ip.toStdString();
//             }
//         }
//         DHCPServer dhcpServer(nodeIPs, as.id);

//         // Simulate a DHCP Discover
//         int nodeId = 1; // Example node ID
//         GenerateMACAddress macaddress;
//         string uniqe_mac = macaddress.generateUniqueMAC();
//         dhcpServer.handleDiscover(nodeId, uniqe_mac);

//         // Simulate a DHCP Request
//         dhcpServer.handleRequest(nodeId);

//         // Print assigned IPs
//         dhcpServer.printAssignedIPs();
//     }

//     return 0;
// }






#include <iostream>
#include <unordered_map>
#include <string>
#include "dhcpserver.h"
#include "macaddressgenerator.h"



// Function to determine AS number based on IP
// int getASNumber(const std::string& ip) {
//     if (ip.find("192.168.100.") != std::string::npos) {
//         return 1; // AS1
//     } else if (ip.find("192.168.200.") != std::string::npos) {
//         return 2; // AS2
//     }
//     return 0; // Unknown AS
// }
// int main() {


//     // Save DHCP allocations
//     router5->saveDhcpAllocations();

    // Output the assigned IP of Router 24

//     return 0;
// }
// #include "router.h"
// #include "rip.h"
// #include "port.h"
// #include <memory>
// #include <thread>

// int main() {
//     // Initialize routers
//     auto router1 = std::make_shared<Router>(1, "192.168.1.1", "00:11:22:33:44:55",1);
//     auto router2 = std::make_shared<Router>(2, "192.168.2.1", "00:11:22:33:44:56",1);
//     auto router3 = std::make_shared<Router>(3, "192.168.3.1", "00:11:22:33:44:57",1);

//     // Initialize ports
//     auto port1 = std::make_shared<Port>();
//     auto port2 = std::make_shared<Port>();
//     auto port3 = std::make_shared<Port>();

//     // Set port numbers and owners
//     port1->setNumber(1);
//     port1->setOwnerNode(router1);
//     port1->setRouterIP("192.168.1.1");

//     port2->setNumber(2);
//     port2->setOwnerNode(router2);
//     port2->setRouterIP("192.168.2.1");

//     port3->setNumber(3);
//     port3->setOwnerNode(router3);
//     port3->setRouterIP("192.168.3.1");

//     // Connect ports
//     port1->connectTo(port2.get());
//     port2->connectTo(port3.get());

//     // Assign ports to routers
//     router1->addPort(port1);
//     router2->addPort(port2);
//     router3->addPort(port3);

//     // Initialize RIP protocols for each router
//     RIP ripRouter1(router1.get());
//     RIP ripRouter2(router2.get());
//     RIP ripRouter3(router3.get());

//     // Run RIP protocol for each router in separate threads
//     std::thread thread1([&]() { ripRouter1.initialize(); });
//     std::thread thread2([&]() { ripRouter2.initialize(); });
//     std::thread thread3([&]() { ripRouter3.initialize(); });

//     // Wait for RIP protocols to complete
//     thread1.join();
//     thread2.join();
//     thread3.join();

//     // Display routing tables for all routers
//     std::cout << "Routing table for Router 1:\n";
//     ripRouter1.displayRoutingTable();

//     std::cout << "\nRouting table for Router 2:\n";
//     ripRouter2.displayRoutingTable();

//     std::cout << "\nRouting table for Router 3:\n";
//     ripRouter3.displayRoutingTable();
//     std::string serializedTable = ripRouter1.serializeRoutingTable();
//     std::cout << "Serialized Routing Table: " << serializedTable << "\n";
//     return 0;
// }




#include <iostream>
#include <fstream>
#include <memory>
#include "topolgybuilder.h"
#include "C:/Users/Entezar/Documents/CA3/include/json.hpp"

using json = nlohmann::json;

int main() {
    // Load configuration file
    std::ifstream configFile("C:/Users/Entezar/Documents/CA3/config.json");
    if (!configFile.is_open()) {
        std::cerr << "Failed to open config file.\n";
        return 1;
    }

    json config;
    configFile >> config;

    // Initialize topology builder
    TopologyBuilder builder;

    // Process each Autonomous System
    for (const auto& asConfig : config["Autonomous_systems"]) {
        int asId = asConfig["id"];
        int dhcpServerId = asConfig["dhcp_server"];
        std::string ipRange = asConfig["ip_range"];

        // Create the DHCP server router
        std::shared_ptr<Router> dhcpRouter = std::make_shared<Router>(dhcpServerId, "", "AA:BB:CC:DD:EE:FF", asId);
        dhcpRouter->enableDhcp(ipRange, asId);
        builder.buildRoutersAndPCs(asConfig,dhcpRouter);
        // Build routers and PCs for the AS and assign IPs
        // builder.buildRouters(asConfig, dhcpRouter);
        // builder.buildPCs(asConfig, dhcpRouter);
        builder.connectAsGateways(asConfig);
        builder.connectPCsToRouters(asConfig);

        // Save DHCP allocations for the AS
        dhcpRouter->saveDhcpAllocations();
    }

    std::cout << "Topology built successfully!" << std::endl;
    return 0;
}



