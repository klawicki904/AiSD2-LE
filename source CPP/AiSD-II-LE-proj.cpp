// Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.

#include "Field.h"
//#include "Brewery.h"
//#include "Pub.h"
//#include "Map.h"
//#include "Town.h"

#include <iostream>

using namespace std;

int main()
{
	Field field1 = Field(1, 2, 1234);
	cout << "Hello\n"<< field1.GetX() << ", " << field1.GetY() << ", " << field1.GetYield();
}