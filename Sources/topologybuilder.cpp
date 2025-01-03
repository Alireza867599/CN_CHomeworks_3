#include <fstream>
#include <iostream>
#include <stdexcept>
#include "topolgybuilder.h"
#include "macaddressgenerator.h"
// Load the configuration file
void TopologyBuilder::loadConfig(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath);
    if (!configFile) {
        throw std::runtime_error("Failed to open configuration file.");
    }

    json config;
    configFile >> config;

    buildTopology(config);
}

// Build the entire topology
void TopologyBuilder::buildTopology(const json& config) {
    for (const auto& asConfig : config["Autonomous_systems"]) {
        int asId = asConfig["id"];

        // Get the DHCP router for this AS (you can customize this based on your setup)
        std::shared_ptr<Router> dhcpRouter = getDhcpRouterForAs(asId);

        // Build routers, passing the DHCP router
        buildRouters(asConfig, dhcpRouter);

        buildPCs(asConfig,dhcpRouter);
        handleBrokenRouters(asConfig);
        buildASTopology(asConfig);
        connectPCsToRouters(asConfig);
        connectAsGateways(asConfig);
    }
}

std::shared_ptr<Router> TopologyBuilder::getDhcpRouterForAs(int asId) {
    if (asId == 1) {
        return std::make_shared<Router>(5, "", "AA:BB:CC:DD:EE:FF", asId);
    }
    // Add cases for other ASes
    return nullptr;
}
void TopologyBuilder::buildRoutersAndPCs(const json& asConfig, std::shared_ptr<Router> dhcpRouter) {
    int asId = asConfig["id"];
    int routerStartId = (asId == 1) ? 1 : 17;
    int routerCount = asConfig["router_count"];
    int pcStartId = (asId == 1) ? 24 : 32;
    int pcCount = asConfig["pc_count"];

    GenerateMACAddress macAddressGenerator;

    // Create routers
    for (int i = 0; i < routerCount; ++i) {
        int routerId = routerStartId + i;
        if (routers.find(routerId) == routers.end()) {
            std::shared_ptr<Router> router = std::make_shared<Router>(routerId, "", "", asId);

            std::string macAddress = macAddressGenerator.generateUniqueMAC();
            router->setMacaddress(macAddress);
            routers[routerId] = router;
            std::cout << "Router created: ID = " << routerId << " with MAC: " << macAddress << " in AS " << asId << std::endl;
        }
    }

    // Create PCs
    for (int i = 0; i < pcCount; ++i) {
        int pcId = pcStartId + i;
        if (pcs.find(pcId) == pcs.end()) {
            auto pc = std::make_shared<PC>(pcId);

            std::string macAddress = macAddressGenerator.generateUniqueMAC();
            pc->setMacaddress(macAddress);  // Assuming PC class has a setMacAddress method

            pcs[pcId] = pc;
            std::cout << "PC created: ID = " << pcId << " with MAC: " << macAddress << " in AS " << asId << std::endl;
        }
    }

    for (int i = 0; i < routerCount; ++i) {
        int routerId = routerStartId + i;
        auto router = routers[routerId];
        std::cout << "Router ID " << routerId << " assigned IP: " << router->getIPAddress() << "\n";
    }

    for (int i = 0; i < pcCount; ++i) {
        int pcId = pcStartId + i;
        auto pc = pcs[pcId];
        pc->requestIP(dhcpRouter);  // Request IP from DHCP server
        std::cout << "PC ID " << pcId << " assigned IP: " << pc->getIPAddress() << "\n";
    }
}


// Build routers for a specific AS
void TopologyBuilder::buildRouters(const json& asConfig, std::shared_ptr<Router> dhcpRouter) {
    int asId = asConfig["id"];
    int routerStartId = (asId == 1) ? 1 : 17;
    int routerCount = asConfig["router_count"];
    std::string ipRange = asConfig["ip_range"];

    for (int i = -1; i < routerCount; ++i) {
        int routerId = routerStartId + i;

        if (routers.find(routerId) == routers.end()) {
            std::shared_ptr<Router> router = std::make_shared<Router>(routerId, "", "", asId);

            routers[routerId] = router;
            std::cout << "Router created: ID = " << routerId << " in AS " << asId << std::endl;

            // Assign IP using DHCP
            router->requestIP(dhcpRouter);
            std::cout << "Router ID " << routerId << " assigned IP: " << router->getIPAddress() << "\n";
        }
    }
}


void TopologyBuilder::buildPCs(const json& asConfig, std::shared_ptr<Router> dhcpServer) {
    int asId = asConfig["id"];
    int pcStartId = (asId == 1) ? 24 : 32;
    int pcCount = asConfig["pc_count"];

    for (int i = 0; i < pcCount; ++i) {
        int pcId = pcStartId + i;
        if (pcs.find(pcId) == pcs.end()) {
            // Create the PC object
            auto pc = std::make_shared<PC>(pcId);
            pcs[pcId] = pc;

            pc->requestIP(dhcpServer);

            // Log the created PC
            std::cout << "PC created: ID = " << pcId << " in AS " << asId << std::endl;
        }
    }
}


// Handle broken routers for a specific AS
void TopologyBuilder::handleBrokenRouters(const json& asConfig) {
    if (!asConfig.contains("broken_routers")) return;

    for (int brokenRouterId : asConfig["broken_routers"]) {
        if (routers.erase(brokenRouterId)) {
            std::cout << "Broken router removed: ID = " << brokenRouterId << std::endl;
        } else {
            std::cerr << "Warning: Broken router ID = " << brokenRouterId << " not found." << std::endl;
        }
    }
}

// Build topology for a specific AS
void TopologyBuilder::buildASTopology(const json& asConfig) {
    std::string topologyType = asConfig["topology_type"];
    int routerCount = asConfig["router_count"];

    if (topologyType == "Mesh") {
        for (int i = 1; i <= routerCount; ++i) {
            for (int j = i + 1; j <= routerCount; ++j) {
                int router1Id = asConfig["id"].get<int>() * 100 + i;
                int router2Id = asConfig["id"].get<int>() * 100 + j;
                if (routers.find(router1Id) != routers.end() && routers.find(router2Id) != routers.end()) {
                    routers[router1Id]->connectRouter(router2Id, routers[router2Id]);
                    routers[router2Id]->connectRouter(router1Id, routers[router1Id]);
                    std::cout << "Connected Router " << router1Id << " to Router " << router2Id << " in Mesh topology." << std::endl;
                }
            }
        }
    } else if (topologyType == "RingStar") {
        for (int i = 1; i <= routerCount; ++i) {
            int routerId = asConfig["id"].get<int>() * 100 + i;
            int nextRouterId = asConfig["id"].get<int>() * 100 + (i % routerCount) + 1;
            if (routers.find(routerId) != routers.end() && routers.find(nextRouterId) != routers.end()) {
                routers[routerId]->connectRouter(nextRouterId, routers[nextRouterId]);
                std::cout << "Connected Router " << routerId << " to Router " << nextRouterId << " in RingStar topology." << std::endl;
            }
        }
    } else {
        std::cerr << "Unknown topology type: " << topologyType << std::endl;
    }
}

// Connect user gateways for a specific AS
void TopologyBuilder::connectPCsToRouters(const json& asConfig) {
    for (const auto& gateway : asConfig["gateways"]) {
        int routerId = gateway["router"];
        for (const auto& pcId : gateway["pcs"]) {
            if (routers.find(routerId) != routers.end() && pcs.find(pcId) != pcs.end()) {
                routers[routerId]->connectUser(pcId, pcs[pcId]);
                std::cout << "Connected Router " << routerId << " to PC " << pcId << std::endl;
            } else {
                std::cerr << "Error: Router " << routerId << " or PC " << pcId << " not found." << std::endl;
            }
        }
    }
}

// Connect AS gateways
void TopologyBuilder::connectAsGateways(const json& asConfig) {
    if (!asConfig.contains("connect_to_as")) return;

    for (const auto& connection : asConfig["connect_to_as"]) {
        for (const auto& pair : connection["gateway_pairs"]) {
            int gatewayId = pair["gateway"];
            int targetGatewayId = pair["connect_to"];
            if (routers.find(gatewayId) != routers.end() && routers.find(targetGatewayId) != routers.end()) {
                routers[gatewayId]->connectRouter(targetGatewayId, routers[targetGatewayId]);
                std::cout << "Connected Gateway " << gatewayId << " to Target Gateway " << targetGatewayId << std::endl;
            } else {
                std::cerr << "Error: Gateway " << gatewayId << " or target gateway " << targetGatewayId << " not found." << std::endl;
            }
        }
    }
}
void TopologyBuilder::printTopology() const {
    for (const auto& [asId, routers] : autonomousSystems) {
        std::cout << "Autonomous System " << asId << ":\n";
        for (const auto& router : routers) {
            std::cout << "  Router " << router->getId() << " (" << router->getIPAddress() << "):\n";

            std::cout << "    Connected Routers: ";
            for (const auto& connectedRouter : router->getConnectedRouters()) {
                std::cout << connectedRouter->getId() << " ";
            }
            std::cout << "\n";

            std::cout << "    Connected PCs: ";
            for (const auto& connectedPC : router->getConnectedPCs()) {
                std::cout << connectedPC->getIpAddress() << " ";
            }
            std::cout << "\n";
        }
    }
}

