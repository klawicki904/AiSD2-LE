#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include "Node.h"

using namespace std;

/// Field F (pole) - dziedziczy z Node;
/// okre�lona produkcja(w ka�dym tyle samo na �wiartk�) - przepustowo�� z S do F

class Field : public Node
{
private:

public:
    Field(const int& _x, const int& _y, const double& _cap);
    
};

#endif // FIELD_H
