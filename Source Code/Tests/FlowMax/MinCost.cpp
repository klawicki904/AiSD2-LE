
#include "gtest/gtest.h"
#include "../source CPP/Matrix.h"
#include "../source CPP/EdgeData.h"

TEST(MaxFlowClassicGraph2, SimpleGraph2) {
    Matrix m(4);
    m.addEdge(0, 1, 10.0, 2.0);
    m.addEdge(0, 2, 10.0, 2.0);
    m.addEdge(1, 3, 10.0, 2.0);
    m.addEdge(2, 3, 10.0, 2.0);

    double result = m.fordFulkerson();
    EXPECT_DOUBLE_EQ(20.0, result);
}