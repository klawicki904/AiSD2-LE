
#include "gtest/gtest.h"
#include "Node.h"


using namespace std;

double Node::breweryEfficiencyMultiplier = 0.0;

// parameter-less constructor needed for inheritance
Node::Node()
    : x(0), y(0), capacity(-1), type(NodeType::None) {}

Node::Node(const int& _id, const double& _x, const double& _y, const double& _cap, NodeType _type)
    : id(_id), x(_x), y(_y), capacity(_cap), type(_type) {}


void Node::setCapacity(double capacity) {
    this->capacity = capacity;
}

    const int& Node::GetId() const {
        return id;
    }
    const double& Node::GetX() const
    {
        return x; 
    }
    const double& Node::GetY() const
    {
        return y;
    }
    double Node::GetCapacity() const {
        return capacity;
    }
    Node::NodeType Node::GetType() const {
        return type;
    }

