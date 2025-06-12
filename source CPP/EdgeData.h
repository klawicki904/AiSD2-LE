#ifndef EDGE_DATA_H
#define EDGE_DATA_H

#include <vector>

using namespace std;

class EdgeData
{

public:
	int v; // u -> v
	double remainingFlow; //pozostala przepustowosc
	double cost; // koszt naprawy drogi
	double flow; //orginalna przepustowosc dla krawedzi
	//int backwardsFlow; //dla optymalizacji

	EdgeData();

	EdgeData(int v, double remainingFlow);

	EdgeData(int v, double remainingFlow, double cost);

	static EdgeData* findEdge(std::vector<EdgeData>& edges, int to);
};

#endif
