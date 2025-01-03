// RIP.h
#ifndef RIP_H
#define RIP_H

#include <iostream>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <queue>
#include <sstream>
#include "router.h" // Include your Router class

const int INFINITY_COST = std::numeric_limits<int>::max();

class RIP {
private:
    Router* router;                                // Reference to the router
    std::map<int, int> neighbors;                 // Neighbor ID and cost
    std::map<int, RoutingTableEntry> routingTable; // Routing table
    std::mutex routingMutex;                      // Mutex for thread safety
    bool protocolComplete;                        // Protocol completion flag


    std::map<int, RoutingTableEntry> deserializeRoutingTable(const std::string& data);

public:
    RIP(Router* router);
    void processRoutingUpdates(const Packet& packet);
    std::string serializeRoutingTable();
    void identifyNeighbors();
    void sendRoutingUpdates();

    void initialize();
    void processUpdates();
    void displayRoutingTable();
    bool isProtocolComplete() const;
};

#endif // RIP_H
