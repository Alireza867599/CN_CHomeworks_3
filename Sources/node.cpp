#include "node.h"

// Correct definition of the constructor
Node::Node(const std::string &macAddress) : macAddress(macAddress) {}

Node::~Node() {}

std::string Node::getMacAddress() const {
    return macAddress;
}
