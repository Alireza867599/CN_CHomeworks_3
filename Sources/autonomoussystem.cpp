#include "autonomoussystem.h"
#include <iostream>
using namespace std;
AutonomousSystem::AutonomousSystem(int id) : id(id) {}

AutonomousSystem::~AutonomousSystem() {}

int AutonomousSystem::getId() const {
    return id;
}

void AutonomousSystem::addRouter(shared_ptr<Router> router) {
    routers.push_back(router);
    cout << "Added Router with ID: " << router->getId() << " to AS " << id << endl;
}

void AutonomousSystem::addPC(shared_ptr<PC> pc) {
    pcs.push_back(pc);
    cout << "Added PC with ID: " << pc->getId() << " to AS " << id << endl;
}

shared_ptr<Router> AutonomousSystem::getRouter(int id) const {
    for (const auto& router : routers) {
        if (router->getId() == id) {
            return router;
        }
    }
    return nullptr;
}

shared_ptr<PC> AutonomousSystem::getPC(int id) const {
    for (const auto& pc : pcs) {
        if (pc->getId() == id) {
            return pc;
        }
    }
    return nullptr;
}

void AutonomousSystem::setGateway(shared_ptr<Router> gateway) {
    this->gateway = gateway;
    cout << "Set Gateway Router with ID: " << gateway->getId() << " for AS " << id << endl;
}

shared_ptr<Router> AutonomousSystem::getGateway() const {
    return gateway;
}

void AutonomousSystem::connectToAS(shared_ptr<AutonomousSystem> otherAS) {
    connectedAS.push_back(otherAS);
    cout << "Connected AS " << id << " to AS " << otherAS->getId() << endl;
}
