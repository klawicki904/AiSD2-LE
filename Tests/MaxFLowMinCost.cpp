#include "gtest/gtest.h"
#include "../source CPP/Matrix.h"
#include "../source CPP/EdgeData.h"
#include "../source CPP/Node.h"

//Testy metod ktore rozwiazuja problemy z projektu

TEST(MinCostMaxFlowOfficialCase, Case0_SimpleGraph) {
    Matrix m(8);
    m.addEdge(0, 1, 99, 0);
    m.addEdge(0, 2, 99, 0);
    m.addEdge(1, 2, 2, 1);
    m.addEdge(1, 3, 5, 9);
    m.addEdge(2, 4, 2, 1);
    m.addEdge(3, 5, 5, 5);
    m.addEdge(4, 3, 3, 4);
    m.addEdge(5, 6, 3, 3);
    m.addEdge(6, 2, 3, 2);
    m.addEdge(3, 7, 99, 0);
    m.addEdge(4, 7, 99, 0);

    m.addNode(Node(1, 10, 13, 3, Node::NodeType::Field));
    m.addNode(Node(2, 7, 17, 3, Node::NodeType::Field));    
    m.addNode(Node(5, 2, 3, 3, Node::NodeType::Brewery));
    m.addNode(Node(3, 10, 13, 3, Node::NodeType::Pub));
    m.addNode(Node(4, 12, 13, 3, Node::NodeType::Pub));

    double maxFlow = m.maxFlowAlgorithm();
    double minCost = m.maxFlowMinCost2();
    EXPECT_DOUBLE_EQ(5, maxFlow);
    EXPECT_DOUBLE_EQ(2*(1+4+5)+3*(9+5) + 2*(-9)+3*(-5), minCost);
    // EXPECT_DOUBLE_EQ(75, BG_BellmanFordCost);
}