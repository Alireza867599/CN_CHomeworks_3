#ifndef AUTONOMOUSSYSTEM_H
#define AUTONOMOUSSYSTEM_H

#include <vector>
#include <memory>
#include "router.h"
#include "pc.h"
using namespace std;
class AutonomousSystem {
public:
    AutonomousSystem(int id);
    ~AutonomousSystem();

    int getId() const;

    void addRouter(shared_ptr<Router> router);
    void addPC(shared_ptr<PC> pc);
    shared_ptr<Router> getRouter(int id) const;
    shared_ptr<PC> getPC(int id) const;

    void setGateway(shared_ptr<Router> gateway);
    shared_ptr<Router> getGateway() const;

    void connectToAS(shared_ptr<AutonomousSystem> otherAS);

private:
    int id;
    vector<shared_ptr<Router>> routers;
    vector<shared_ptr<PC>> pcs;
    shared_ptr<Router> gateway; // Default gateway for the AS
    vector<shared_ptr<AutonomousSystem>> connectedAS;
};

#endif // AUTONOMOUSSYSTEM_H
