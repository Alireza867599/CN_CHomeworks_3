#include "topologybuilder.h"
#include <iostream>

TopologyBuilder::TopologyBuilder() {}

TopologyBuilder::~TopologyBuilder() {}

void TopologyBuilder::buildMeshTopology(int numRouters, int numPCs) {
    // Clear existing topology
    resetTopology();

    // Create routers
    for (int i = 0; i < numRouters; ++i) {
        auto router = std::make_shared<Router>();
        routers.push_back(router);
    }

    // Connect routers in a full mesh
    for (size_t i = 0; i < routers.size(); ++i) {
        for (size_t j = i + 1; j < routers.size(); ++j) {
            connectNodes(routers[i], routers[j]);
        }
    }

    // Create PCs and connect to routers
    for (int i = 0; i < numPCs; ++i) {
        auto pc = std::make_shared<PC>();
        pcs.push_back(pc);

        // Randomly connect PC to a router
        connectNodes(pc, routers[i % numRouters]);
    }
}

void TopologyBuilder::buildStarRingTopology(int numRouters, int numPCs) {
    // Clear existing topology
    resetTopology();

    // Create routers in a ring
    for (int i = 0; i < numRouters; ++i) {
        auto router = std::make_shared<Router>();
        routers.push_back(router);

        if (i > 0) {
            connectNodes(routers[i - 1], router);
        }
    }
    // Close the ring
    connectNodes(routers[0], routers[numRouters - 1]);

    // Add a central node and connect to the ring
    auto centralRouter = std::make_shared<Router>();
    routers.push_back(centralRouter);
    for (auto& router : routers) {
        connectNodes(centralRouter, router);
    }

    // Create PCs and connect to routers
    for (int i = 0; i < numPCs; ++i) {
        auto pc = std::make_shared<PC>();
        pcs.push_back(pc);

        // Randomly connect PC to a router
        connectNodes(pc, routers[i % numRouters]);
    }
}

void TopologyBuilder::resetTopology() {
    routers.clear();
    pcs.clear();
}

void TopologyBuilder::connectNodes(std::shared_ptr<Node> node1, std::shared_ptr<Node> node2) {
    auto port1 = std::make_shared<Port>();
    auto port2 = std::make_shared<Port>();

    port1->connectTo(port2.get());
    port2->connectTo(port1.get());

    node1->addPort(port1);
    node2->addPort(port2);

    std::cout << "Connected Node " << node1->getId() << " to Node " << node2->getId() << "\n";
}
