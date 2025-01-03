#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <queue>
#include <limits>
#include "router.h" // Include your Router class
#include <sstream>
#include "rip.h"

RIP::RIP(Router* router)
    : router(router), protocolComplete(false) {}

void RIP::identifyNeighbors() {
    for (auto& port : router->getPorts()) {
        if (port->sendhello()) { // Assuming Port class has sendHello()
            int neighborId = port->getConnectedNode()->getId(); // Assuming `Node` has a `getId()` method.
            int cost = port->getCost();
            neighbors[neighborId] = cost;
            RoutingTableEntry entry{
                "192.168." + std::to_string(neighborId) + ".0", // Destination
                "255.255.255.0",                                // Subnet mask
                router->getIPAddress(),                         // Next-hop gateway
                cost,                                           // Metric
                "RIP"                                          // Protocol
            };
            routingTable[neighborId] = entry;
        }
    }
}

void RIP::sendRoutingUpdates() {
    Packet packet(UT::PacketType::Control, nullptr, nullptr, nullptr, "Routing Update", 0, 0, 1);
    for (const auto& [neighborId, cost] : neighbors) {
        RoutingTableEntry neighborRoute = routingTable[neighborId];
        packet.setPayload(serializeRoutingTable());
        router->sendPacket(packet);
    }
}

void RIP::processRoutingUpdates(const Packet& packet) {
    auto receivedTable = deserializeRoutingTable(packet.getPayload());
    for (const auto& [dest, entry] : receivedTable) {
        int newCost = neighbors[packet.getSourceid()] + entry.metric;
        if (routingTable.find(dest) == routingTable.end() || routingTable[dest].metric > newCost) {
            std::lock_guard<std::mutex> lock(routingMutex);
            routingTable[dest] = {
                entry.destinationNetwork, entry.subnetMask,
                entry.gateway, newCost, "RIP"
            };
        }
    }
}

std::string RIP::serializeRoutingTable() {
    std::ostringstream oss;
    for (const auto& [id, entry] : routingTable) {
        oss << entry.destinationNetwork << "," << entry.subnetMask << ","
            << entry.gateway << "," << entry.metric << ";";
    }
    return oss.str();
}

// Moved deserializeRoutingTable inside RIP class
std::map<int, RoutingTableEntry> RIP::deserializeRoutingTable(const std::string& data) {
    std::map<int, RoutingTableEntry> table;
    std::istringstream iss(data);
    std::string token;

    while (std::getline(iss, token, ';')) {
        if (token.empty()) continue; // Skip empty entries
        std::istringstream entryStream(token);
        std::string dest, subnet, gateway;
        int metric;

        try {
            std::getline(entryStream, dest, ',');
            std::getline(entryStream, subnet, ',');
            std::getline(entryStream, gateway, ',');
            std::string metricStr;
            std::getline(entryStream, metricStr, ',');
            metric = std::stoi(metricStr); // Ensure this is numeric

            table[table.size()] = {dest, subnet, gateway, metric, "RIP"};
        } catch (const std::exception& e) {
            std::cerr << "Error parsing entry: " << token << "\nException: " << e.what() << "\n";
        }
    }

    return table;
}


void RIP::initialize() {
    identifyNeighbors();
    sendRoutingUpdates();
    processUpdates();
}

void RIP::processUpdates() {
    auto startTime = std::chrono::steady_clock::now();
    while (true) {
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count() > 60) {
            protocolComplete = true;
            break;
        }

        Packet packet = router->retrievePacketFromBuffer();
        if (!packet.getPayload().empty() && packet.getPacketType() == UT::PacketType::Control) {
            processRoutingUpdates(packet);
        }
    }
}

void RIP::displayRoutingTable() {
    std::lock_guard<std::mutex> lock(routingMutex);
    router->displayRoutingTable();
}

bool RIP::isProtocolComplete() const {
    return protocolComplete;
}
