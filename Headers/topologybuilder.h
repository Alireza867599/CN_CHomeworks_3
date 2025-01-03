#ifndef TOPOLOGYBUILDER_H
#define TOPOLOGYBUILDER_H

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "C:/Users/Entezar/Documents/CA3/include/json.hpp"
#include "router.h"
#include "pc.h"
#include "autonomoussystem.h"
using json = nlohmann::json;

class TopologyBuilder {
public:
    void loadConfig(const std::string& configFilePath);
    void buildTopology(const json& config);
    void printTopology() const ;
    void buildRouters(const json& asConfig, std::shared_ptr<Router> dhcpRouter);
    void connectUserGateways(const json& asConfig);
    void connectAsGateways(const json& asConfig);
    void connectPCsToRouters(const json& asConfig);
    void buildPCs(const json& asConfig, std::shared_ptr<Router> dhcpServer);
    void buildRoutersAndPCs(const json& asConfig, std::shared_ptr<Router> dhcpRouter);

private:
    std::shared_ptr<Router> getDhcpRouterForAs(int asId);

    std::unordered_map<int, std::shared_ptr<Router>> routers;
    std::unordered_map<int, std::shared_ptr<PC>> pcs;
    std::unordered_map<int, std::vector<std::shared_ptr<Router>>> autonomousSystems;
    void buildTopology1(const json& config);

    void buildASTopology(const json& asConfig);

    void handleBrokenRouters(const json& asConfig);



};

#endif // TOPOLOGY_BUILDER_H
