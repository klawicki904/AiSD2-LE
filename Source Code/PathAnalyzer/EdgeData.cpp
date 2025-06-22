#include "gtest/gtest.h"
#include "EdgeData.h"



EdgeData::EdgeData() : v(), remainingFlow(0), flow(0), cost(0) {}

EdgeData::EdgeData(int v, double remainingFlow) {
	this->v = v;
	this->remainingFlow = remainingFlow;
	cost = 0.0;
	this->flow = remainingFlow;
}

EdgeData::EdgeData(int v, double remainingFlow, double cost) {
	this->v = v;
	this->remainingFlow = remainingFlow;
	this->cost = cost;
	this->flow = remainingFlow;
}

EdgeData* EdgeData::findEdge(std::vector<EdgeData>& edges, int to) {
	for (auto& edge : edges) {
		if (edge.v == to) return &edge;
	}
	return nullptr;
}