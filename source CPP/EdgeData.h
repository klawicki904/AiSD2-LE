#ifndef EDGE_DATA_H
#define EDGE_DATA_H


class EdgeData
{

public:
	int v; // u -> v
	double remainingFlow;
	//int maxFlow;
	//int backwardsFlow;

	EdgeData();

	EdgeData(int v, double remainingFlow);
};

#endif
