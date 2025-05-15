#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

#include <iostream>
#include "EdgeData.h"
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

class AdjacencyList
{
public:
	vector<vector<EdgeData>> nList;
	int vertices;

	AdjacencyList();
	AdjacencyList(int n);

	void addEdge(int u, int v, double value);

	bool readFileToGraph(string fileName);

	void printGraph();

	void bfsFlow(vector<vector<EdgeData>>& aList, vector<int>& parents, vector<double>& flows, int start );

	double edmonsKarp();
};

#endif
