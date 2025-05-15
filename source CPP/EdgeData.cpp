#include "gtest/gtest.h"
#include "EdgeData.h"


EdgeData::EdgeData() : v(), remainingFlow(0) {}

EdgeData::EdgeData(int v, double remainingFlow) {
	this->v = v;
	this->remainingFlow = remainingFlow;
	cost = 0.0;
}

EdgeData::EdgeData(int v, double remainingFlow, double cost) {
	this->v = v;
	this->remainingFlow = remainingFlow;
	this->cost = cost;
}