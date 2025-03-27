#ifndef FIELD_H
#define FIELD_H

#include <iostream>

using namespace std;

/// Pola - wierzcho�ki startowe (�r�d�a):
/// a) po�o�enie
/// b) ile wyrasta j�czmienia �rednio (w ka�dym tyle samo na �wiartk�)

class Field
{
private:
    int x;
    int y;
    double yield = 0.0;

public:
    Field(const int& _x, const int& _y, const double& _yield);

    const int& GetX() const;
    const int& GetY() const;
    const double& GetYield() const;
    //const double& GetYield() const;
    
};

#endif // FIELD_H
