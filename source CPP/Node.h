#ifndef NODE_H
#define NODE_H

#include <iostream>

using namespace std;

/*
Node (w�ze�):
a) po�o�enie(punkt na p�aszczy�nie)
b) maksymalna przepustowo��
�r�d�o S - pocz�tkowa rezerwa j�czmienia
uj�cie T - ko�cowy odbi�r piwa przez karczmy
�r�d�o S i uj�cie T to b�d� Node, kt�re nie maj� limit�w przepustowo�ci
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
    Node(const int& _id, const double& _x, const double& _y, const double& _cap, NodeType _type = NodeType::None);

    void setCapacity(double capacity);
    const int& GetId() const;
    const double& GetX() const;
    const double& GetY() const;
    double GetCapacity() const;
    NodeType GetType() const;
    //const double& GetYield() const;

    wstring TypeAsWString() const {
        switch (type) {
        case NodeType::Field: return L"pole";
        case NodeType::Brewery: return L"browar";
        case NodeType::Pub: return L"karczma";
        default: return L"skrzy?owania";
        }
    }

};

#endif // NODE_H