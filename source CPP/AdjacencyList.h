#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <iostream>
#include "EdgeData.h"
#include <vector>
#include <fstream>
#include <queue>
#include "Node.h"
#include "Path.h"
#include <algorithm> //korzystam z reverse dla listy

using namespace std;

class AdjacencyList
{
public:
	vector<vector<EdgeData>> nList;
	int vertices;
	int source; // ?ród?o
	int target; // Uj?cie
	vector<Node> listVertices; // Zawiera informacje o wierzcho?kach

	AdjacencyList();
	AdjacencyList(int n);

	void addEdge(int u, int v, double value, double cost);

	bool readFileToGraph(string fileName);

	void printGraph();

	void bfsFlow(vector<vector<EdgeData>>& aList, vector<int>& parents, vector<double>& flows, int start );

	double edmonsKarp();

	void createResidualNetwork(AdjacencyList& temp);

	bool dijkstraModify(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf);

	bool BellmanFord(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf);

	double BusackerGowen2(double const maxFlow, int s, int t, vector<Path>& roads,
		bool (AdjacencyList::* shortestPathFunc)(int, int, double&, vector<int>&, const vector<vector<EdgeData>>&));

	void maxFlowMinCost();

};

#endif
