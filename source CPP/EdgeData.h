#ifndef EDGE_DATA_H
#define EDGE_DATA_H


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
};

#endif
