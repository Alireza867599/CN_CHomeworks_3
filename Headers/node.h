#ifndef NODE_H
#define NODE_H

#include <QThread>
#include <string>

class Node : public QThread {
protected:
    std::string macAddress;

public:
    explicit Node(const std::string &macAddress); // Ensure explicit matches
    virtual ~Node();
    std::string getMacAddress() const;
};

#endif // NODE_H
