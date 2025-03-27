#include "Field.h"

using namespace std;

Field::Field(const int& _x, const int& _y, const double& _yield)
    : x(_x), y(_y), yield(_yield){}

    const int& Field::GetX() const
    {
        return x;
    }
    const int& Field::GetY() const
    {
        return y;
    }
    const double& Field::GetYield() const
    {
        return yield;
    }

