#include "topologycontroller.h"
#include <iostream>

using namespace std;


TopologyController::TopologyController() : builder(nullptr) {}

TopologyController::~TopologyController() {}

void TopologyController::setBuilder(shared_ptr<TopologyBuilder> builder) {
    this->builder = builder;
}

void TopologyController::addRouter() {
    if (!builder) {
        cerr << "TopologyBuilder is not set!" << endl;
        return;
    }
    auto newRouter = make_shared<Router>();
    builder->routers.push_back(newRouter);
    cout << "Added a new Router with ID: " << newRouter->getId() << endl;
}

void TopologyController::addPC() {
    if (!builder) {
        cerr << "TopologyBuilder is not set!" << endl;
        return;
    }
    auto newPC = make_shared<PC>();
    builder->pcs.push_back(newPC);
    cout << "Added a new PC with ID: " << newPC->getId() << endl;
}

void TopologyController::removeNode(int nodeId) {
    if (!builder) {
        cerr << "TopologyBuilder is not set!" << endl;
        return;
    }
    builder->routers.erase(remove_if(builder->routers.begin(), builder->routers.end(),
        [nodeId](const shared_ptr<Router>& router) {
            return router->getId() == nodeId;
        }), builder->routers.end());

    builder->pcs.erase(remove_if(builder->pcs.begin(), builder->pcs.end(),
        [nodeId](const shared_ptr<PC>& pc) {
            return pc->getId() == nodeId;
        }), builder->pcs.end());

    cout << "Removed Node with ID: " << nodeId << endl;
}

void TopologyController::updateConnection(int nodeId1, int nodeId2) {
    if (!builder) {
        cerr << "TopologyBuilder is not set!" << endl;
        return;
    }

    auto node1 = findNodeById(nodeId1);
    auto node2 = findNodeById(nodeId2);

    if (!node1 || !node2) {
        cerr << "One or both nodes not found!" << endl;
        return;
    }

    builder->connectNodes(node1, node2);
    cout << "Connected Node " << nodeId1 << " to Node " << nodeId2 << endl;
}

int TopologyController::getRouterCount() const {
    return builder ? builder->routers.size() : 0;
}

int TopologyController::getPCCount() const {
    return builder ? builder->pcs.size() : 0;
}

bool TopologyController::isConnected(int nodeId1, int nodeId2) const {
    auto node1 = findNodeById(nodeId1);
    auto node2 = findNodeById(nodeId2);

    if (!node1 || !node2) {
        return false;
    }

    for (const auto& port : node1->getPorts()) {
        if (port->isConnected() && port->getConnectedNode()->getId() == nodeId2) {
            return true;
        }
    }
    return false;
}

void TopologyController::printTopology() const {
    if (!builder) {
        cerr << "TopologyBuilder is not set!" << endl;
        return;
    }

    cout << "Topology Details:" << endl;
    for (const auto& router : builder->routers) {
        cout << "Router " << router->getId() << " connected to: ";
        for (const auto& port : router->getPorts()) {
            if (port->isConnected()) {
                cout << port->getConnectedNode()->getId() << ", ";
            }
        }
        cout << endl;
    }

    for (const auto& pc : builder->pcs) {
        cout << "PC " << pc->getId() << " connected to: ";
        for (const auto& port : pc->getPorts()) {
            if (port->isConnected()) {
                cout << port->getConnectedNode()->getId() << ", ";
            }
        }
        cout << endl;
    }
}

shared_ptr<Node> TopologyController::findNodeById(int nodeId) const {
    for (const auto& router : builder->routers) {
        if (router->getId() == nodeId) {
            return router;
        }
    }

    for (const auto& pc : builder->pcs) {
        if (pc->getId() == nodeId) {
            return pc;
        }
    }

    return nullptr;
}
