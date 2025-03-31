
#include "EdgeData.h"


EdgeData::EdgeData() : v(), remainingFlow(0) {}

EdgeData::EdgeData(int v, int remainingFlow) : v(v), remainingFlow(remainingFlow) {}