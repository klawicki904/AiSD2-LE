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
