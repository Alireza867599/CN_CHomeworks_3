#ifndef NODE_H
#define NODE_H

#include <QThread>
#include <string>
#include <memory> // For shared_ptr
#include "port.h"
class DHCP; // Forward declaration

class Node : public QThread {
protected:
    std::string macAddress;
    std::string ipAddress;   // Assigned IP address
    int id;
    std::shared_ptr<DHCP> dhcpServer;  // Pointer to the DHCP server for IP allocation
    std::vector<std::shared_ptr<Port>> m_ports; // Container to store port

public:
    explicit Node();
    virtual ~Node();
    void addPort(std::shared_ptr<Port> port);

    // Getter and setter methods
    std::string getMacAddress() const;
    void setIPAddress(const std::string &ip);
    std::string getIPAddress() const;
    int getId() const;
    void setId(int newId);
    std::vector<std::shared_ptr<Port>> getports();

    // Request IP from DHCP server
    void requestIP();

    std::vector<std::shared_ptr<Port> > ports() const;
    void setPorts(const std::vector<std::shared_ptr<Port> > &newPorts);

private:
};

#endif // NODE_H
