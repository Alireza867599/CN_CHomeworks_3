#ifndef TOPOLOGYBUILDER_H
#define TOPOLOGYBUILDER_H

#include <vector>
#include <memory>
#include "router.h"
#include "pc.h"
#include "port.h"

class TopologyBuilder {
public:
    TopologyBuilder();
    ~TopologyBuilder();

    void buildMeshTopology(int numRouters, int numPCs);
    void buildStarRingTopology(int numRouters, int numPCs);
    void resetTopology();

private:
    std::vector<std::shared_ptr<Router>> routers;
    std::vector<std::shared_ptr<PC>> pcs;

    void connectNodes(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2);
    void setupPorts(std::shared_ptr<Router> router, int numPorts);
};

#endif // TOPOLOGYBUILDER_H
