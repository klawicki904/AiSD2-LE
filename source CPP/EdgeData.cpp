
#include "EdgeData.h"


EdgeData::EdgeData() : v(), remainingFlow(0) {}

EdgeData::EdgeData(int v, double remainingFlow) : v(v), remainingFlow(remainingFlow) {}