#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <iostream>
#include "EdgeData.h"
#include <vector>
#include <fstream>
#include <queue>
#include "Node.h"

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

	void addEdge(int u, int v, double value);

	bool readFileToGraph(string fileName);

	void printGraph();

	void bfsFlow(vector<vector<EdgeData>>& aList, vector<int>& parents, vector<double>& flows, int start );

	double edmonsKarp();
};

#endif
