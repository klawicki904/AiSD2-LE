#ifndef EDGE_DATA_H
#define EDGE_DATA_H


class EdgeData
{

public:
	int v; // u -> v
	int remainingFlow;
	//int maxFlow;
	//int backwardsFlow;

	EdgeData();

	EdgeData(int v, int remainingFlow);
};

#endif
