#include <iostream>
#include "AdjacencyList.h"
#include "Matrix.h"

using namespace std;

int main(int argc, char** argv)
{
    AdjacencyList nList;
    Matrix graphMatrix , matrix2;
    
    if (argc == 1)
    {
        /*if (!matrix2.readFileToGraph3("./daneEksperymentalne.txt"))
        {
            return -1;
        }*/
        if (!graphMatrix.readFileToGraph3("./daneZeWszystkim.txt"))
        {
            return -1;
        }
        graphMatrix.maxFlowMinCost2("./wynik.txt");
    }
    else if (argc == 3)
    {
        if (!graphMatrix.readFileToGraph3(argv[1]))
        {
            return -1;
        }
        graphMatrix.maxFlowMinCost2(argv[2]);
    }
    else
    {
        cerr << "Nieobslugiwana ilosc parametrow uruchomienia\n";
    }
    

    
    //matrix2.maxFlowMinCost2();
    
    


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