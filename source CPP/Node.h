#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

/*
Node (wêze³):
a) po³o¿enie(punkt na p³aszczyŸnie)
b) maksymalna przepustowoœæ
Ÿród³o S - pocz¹tkowa rezerwa jêczmienia
ujœcie T - koñcowy odbiór piwa przez karczmy
Ÿród³o S i ujœcie T to bêd¹ Node, które nie maj¹ limitów przepustowoœci
*/

class Node
{
public:
    enum class NodeType {
        None,
        Field,
        Brewery,
        Pub
    };

    int id;
    double x;       //podmienilem x i y na double
    double y;
    double capacity = -1;
    static double breweryEfficiencyMultiplier;
    NodeType type = NodeType::None;

    Node();
    Node(const int& _id, const int& _x, const int& _y, const double& _cap, NodeType _type = NodeType::None);

    const int& GetId() const;
    const int& GetX() const;
    const int& GetY() const;
    const double& GetCapacity() const;
    NodeType GetType() const;
    //const double& GetYield() const;

    std::wstring TypeAsWString() const {
        switch (type) {
            case NodeType::Field: return L"pole";
            case NodeType::Brewery: return L"browar";
            case NodeType::Pub: return L"karczma";
            default: return L"punkt";
        }
    }

};

#endif // NODE_H
