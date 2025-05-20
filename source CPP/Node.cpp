
#include "gtest/gtest.h"
#include "Node.h"


using namespace std;

// parameter-less constructor needed for inheritance
Node::Node()
    : x(0), y(0), capacity(-1), type(NodeType::None) {}

Node::Node(const int& _id, const int& _x, const int& _y, const double& _cap, NodeType _type)
    : id(_id), x(_x), y(_y), capacity(_cap), type(_type) {}


    const int& Node::GetId() const {
        return id;
    }
    const int& Node::GetX() const
    {
        return x; 
    }
    const int& Node::GetY() const
    {
        return y;
    }
    const double& Node::GetCapacity() const
    {
        return capacity;
    }
    Node::NodeType Node::GetType() const {
        return type;
    }

