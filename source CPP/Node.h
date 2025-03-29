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
protected:
    

public:
    int x;
    int y;
    double capacity = 0.0;
    Node();
    Node(const int& _x, const int& _y, const double& _cap);

    const int& GetX() const;
    const int& GetY() const;
    const double& GetCapacity() const;
    //const double& GetYield() const;
    
};

#endif // NODE_H
