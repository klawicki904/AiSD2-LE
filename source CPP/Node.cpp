#include "Node.h"

using namespace std;

Node::Node() : x(0), y(0), capacity(0.0) {}

Node::Node(const int& _x, const int& _y, const double& _cap)
    : x(_x), y(_y), capacity(_cap){}

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

