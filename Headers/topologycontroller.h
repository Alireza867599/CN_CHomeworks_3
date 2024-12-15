#ifndef TOPOLOGYCONTROLLER_H
#define TOPOLOGYCONTROLLER_H

#include <vector>
#include <memory>
#include "router.h"
#include "pc.h"
#include "topologybuilder.h"

class TopologyController {
public:
    TopologyController();
    ~TopologyController();

    void setBuilder(std::shared_ptr<TopologyBuilder> builder);

    void addRouter();
    void addPC();
    void removeNode(int nodeId);
    void updateConnection(int nodeId1, int nodeId2);

    int getRouterCount() const;
    int getPCCount() const;
    bool isConnected(int nodeId1, int nodeId2) const;
    void printTopology() const;

private:
    std::shared_ptr<TopologyBuilder> builder;

    std::shared_ptr<Node> findNodeById(int nodeId) const;
};

#endif // TOPOLOGYCONTROLLER_H
