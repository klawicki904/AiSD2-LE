#include "gtest/gtest.h"
#include "../source CPP/Matrix.h"
#include "../source CPP/EdgeData.h"
#include "../source CPP/Node.h"

//Testy metod ktore rozwiazuja problemy z projektu


//Sytuacja w której ?cie?ki z pól prowadz? do pubów, a dopiero pó?niej do browarów
TEST(MinCostMaxFlowOfficialCase, Case0_TrickyGraph) {
    Matrix m(7);

    m.addEdge(1, 2, 2, 1);
    m.addEdge(1, 3, 5, 9);
    m.addEdge(2, 4, 2, 1);
    m.addEdge(3, 5, 5, 5);
    m.addEdge(4, 3, 3, 4);
    m.addEdge(5, 2, 3, 3);

    m.addNode(Node(1, 10, 13, 3, Node::NodeType::Field));
    m.addNode(Node(2, 7, 17, 3, Node::NodeType::Field));    
    m.addNode(Node(5, 2, 3, 3, Node::NodeType::Brewery));
    m.addNode(Node(3, 10, 13, 3, Node::NodeType::Pub));
    m.addNode(Node(4, 12, 13, 3, Node::NodeType::Pub));
    m.listVertices[2].capacity = 30;
    m.listVertices[1].capacity = 30;
    m.listVertices[5].capacity = 30;
    Node::breweryEfficiencyMultiplier = 1;
    Matrix siecResidualna(m.vertices);
    vector<Path> road;
    //Tworzy sie? residualn? z?o?on? z 2 warstwa jako jeden graf
    siecResidualna.createResidualNet(m.tab, m.listVertices);

    //Oblicza max flow dla tej sieci residualne
    double maxFlow = siecResidualna.edmondsKarpClassicWithConversion(siecResidualna.source, siecResidualna.target, Node::breweryEfficiencyMultiplier, siecResidualna.vertices - 2);

    EXPECT_DOUBLE_EQ(2, maxFlow);

}

//Sytuacja w której ?cie?ki z pól prowadz? do pubów, a dopiero pó?niej do browarów z konwersja
TEST(MinCostMaxFlowOfficialCase, Case1_TrickyGraphKonwersja) {
    Matrix m(7);

    m.addEdge(1, 2, 2, 1);
    m.addEdge(1, 3, 5, 9);
    m.addEdge(2, 4, 2, 1);
    m.addEdge(3, 5, 5, 5);
    m.addEdge(4, 3, 3, 4);
    m.addEdge(5, 2, 3, 3);

    m.addNode(Node(1, 10, 13, 3, Node::NodeType::Field));
    m.addNode(Node(2, 7, 17, 3, Node::NodeType::Field));
    m.addNode(Node(5, 2, 3, 3, Node::NodeType::Brewery));
    m.addNode(Node(3, 10, 13, 3, Node::NodeType::Pub));
    m.addNode(Node(4, 12, 13, 3, Node::NodeType::Pub));
    m.listVertices[2].capacity = 30;
    m.listVertices[1].capacity = 30;
    m.listVertices[5].capacity = 30;
    Node::breweryEfficiencyMultiplier = 0.25;
    Matrix siecResidualna(m.vertices);
    vector<Path> road;
    //Tworzy sie? residualn? z?o?on? z 2 warstwa jako jeden graf
    siecResidualna.createResidualNet(m.tab, m.listVertices);

    //Oblicza max flow dla tej sieci residualne
    double maxFlow = siecResidualna.edmondsKarpClassicWithConversion(siecResidualna.source, siecResidualna.target, Node::breweryEfficiencyMultiplier, siecResidualna.vertices - 2);

    EXPECT_DOUBLE_EQ(1.25, maxFlow);


}