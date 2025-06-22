
#include "gtest/gtest.h"
#include "../source CPP/Matrix.h"
#include "../source CPP/EdgeData.h"
#include "../source CPP/Quarter.h"
#include "../source CPP/Path.h"
#include "../source CPP/Node.h"

//Testy podstawowych algorytmow (takich ktore mozna sprawdzic na roznych stronach)

TEST(MinCostMaxFlowTest, Case0_FourNodeGraphWithoutMaxFlow_FlowAndCost) {
    Matrix m(4);
    m.addEdge(0, 1, 18, 3);
    m.addEdge(2, 1, 20, 8);
    m.addEdge(2, 3, 15, 4);

    double maxFlow = m.edmondsKarp();
    EXPECT_DOUBLE_EQ(0, maxFlow);

}

TEST(MinCostMaxFlowTest, Case1_SixNodeGraph_FlowAndCost) {
    Matrix m(6);
    m.addEdge(0, 1, 4, 1);
    m.addEdge(0, 2, 5, 9);
    m.addEdge(1, 2, 2, 1);
    m.addEdge(1, 3, 5, 9);
    m.addEdge(2, 4, 2, 1);
    m.addEdge(3, 5, 5, 1);
    m.addEdge(4, 3, 3, 1);
    m.addEdge(4, 5, 3, 9);

    double maxFlow = m.edmondsKarp();
    EXPECT_DOUBLE_EQ(6, maxFlow);

}
TEST(MinCostMaxFlowTest, Case2_FiveNodeGraph_FlowAndCost) {
    Matrix m(5);
    m.addEdge(0, 1, 18, 3);
    m.addEdge(0, 2, 20, 8);
    m.addEdge(1, 2, 15, 4);
    m.addEdge(1, 3, 20, 5);
    m.addEdge(2, 3, 12, 8);
    m.addEdge(3, 4, 14, 5);
    m.addEdge(2, 4, 17, 3);

    double maxFlow = m.edmondsKarp();
    EXPECT_DOUBLE_EQ(31, maxFlow);
}

TEST(MinCostMaxFlowTest, Case3_15NodeGraph_FlowAndCost) {
    Matrix m(15);
    m.addEdge(0, 1, 99, 4);
    m.addEdge(0, 2, 99, 24);
    m.addEdge(0, 3, 99, 1);
    m.addEdge(1, 4, 19, 23);
    m.addEdge(1, 5, 14, 12);
    m.addEdge(1, 7, 20, 1);
    m.addEdge(2, 4, 5, 12);
    m.addEdge(2, 5, 18, 9);
    m.addEdge(2, 6, 13, 3);
    m.addEdge(3, 5, 18, 4);
    m.addEdge(3, 6, 7, 2);
    m.addEdge(4, 9, 11, 3);
    m.addEdge(4, 10, 7, 4);
    m.addEdge(5, 8, 20, 3);
    m.addEdge(5, 9, 8, 5);
    m.addEdge(6, 9, 6, 55);
    m.addEdge(6, 10, 17, 53);
    m.addEdge(7, 8, 20, 33);
    m.addEdge(7, 10, 14, 21);
    m.addEdge(8, 12, 14, 31);
    m.addEdge(8, 13, 7, 43);
    m.addEdge(9, 11, 15, 53);
    m.addEdge(9, 13, 13, 34);
    m.addEdge(10, 11, 17, 23);
    m.addEdge(10, 12, 17, 21);
    m.addEdge(10, 13, 6, 43);
    m.addEdge(11, 14, 99, 1);
    m.addEdge(12, 14, 99, 1);
    m.addEdge(13, 14, 99, 1);

    double maxFlow = m.edmondsKarp();
    EXPECT_DOUBLE_EQ(81, maxFlow);

}







TEST(MinCostMaxFlowTest, Case4_GFG_ClassicalGraph_FlowAndCost) {
    Matrix m(6);
    m.addEdge(0, 1, 16, 0);
    m.addEdge(0, 2, 13, 0);
    m.addEdge(1, 2, 10, 0);
    m.addEdge(1, 3, 12, 0);
    m.addEdge(2, 1, 4, 0);
    m.addEdge(2, 4, 14, 0);
    m.addEdge(3, 2, 9, 0);
    m.addEdge(3, 5, 20, 0);
    m.addEdge(4, 3, 7, 0);
    m.addEdge(4, 5, 4, 0);

    double maxFlow = m.edmondsKarp();
    EXPECT_DOUBLE_EQ(23, maxFlow);
}



TEST(MinCostMaxFlowTest, Case5_GraphWithBackEdges_FlowAndCost) {
    Matrix m(4);
    m.addEdge(0, 1, 10, 0);
    m.addEdge(1, 2, 5, 0);
    m.addEdge(2, 1, 15, 0);
    m.addEdge(2, 3, 10, 0);

    double maxFlow = m.edmondsKarp();
    EXPECT_DOUBLE_EQ(10, maxFlow);
}

TEST(MinCostMaxFlowTest, Case6_NoPathGraph_FlowAndCost) {
    Matrix m(4);
    m.addEdge(0, 1, 10, 0);
    m.addEdge(2, 3, 10, 0); //brak sciezki z 0 do 3

    double maxFlow = m.edmondsKarp();
    EXPECT_DOUBLE_EQ(0, maxFlow);
}

TEST(MinCostMaxFlowTest, Case7_LargeCapacities_FlowAndCost) {
    Matrix m(4);
    m.addEdge(0, 1, 1e6, 0);
    m.addEdge(1, 2, 1e6, 0);
    m.addEdge(2, 3, 1e6, 0);

    double maxFlow = m.edmondsKarp();
    EXPECT_DOUBLE_EQ(1e6, maxFlow);
}

