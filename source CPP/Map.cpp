#include <iostream>
#include "AdjacencyList.h"
#include "Matrix.h"

using namespace std;

int main()
{
    AdjacencyList nList;
    Matrix graphMatrix , matrix2;
    
    
    if (!matrix2.readFileToGraph3("./daneEksperymentalne.txt"))
    {
        return -1;
    }
    if (!graphMatrix.readFileToGraph3("./daneZeWszystkim.txt"))
    {
        return -1;
    }

    
    matrix2.maxFlowMinCost2();
    graphMatrix.maxFlowMinCost2();
    


    //if (!nList.readFileToGraph("./daneMinCost.txt"))
    //{
    //    return -1;
    //}

    //if (!graphMatrix.readFileToGraph("./daneMinCost.txt"))
    //{
    //    return -1;
    //}

    //nList.maxFlowMinCost();
    //cout << "next" << endl;
    //graphMatrix.maxFlowMinCost();
   // matrix2.printGraph();
    return 0;
}