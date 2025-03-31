#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() {
    vertices = 0;
    nList.clear();
}

AdjacencyList::AdjacencyList(int n) {
	vertices = n;
	nList.resize(n);
}


void AdjacencyList::addEdge(int u, int v, double value) {
	nList[u].push_back(EdgeData(v, value));
}

bool AdjacencyList::readFileToGraph(string fileName) {
    ifstream plik(fileName);
    int vertices;
    int newEdge1, newEdge2, maxFlow;
    string line;

    if (!(plik >> vertices >> newEdge1))
    {
        cerr << "Nie można otworzyć pliku ";
        return false;
    }

        this->vertices = vertices;
        this->nList.resize(vertices);

        while (plik >> newEdge1 >> newEdge2 >> maxFlow)
        {
            addEdge(newEdge1, newEdge2, maxFlow);
        }
        plik.close();
        return true;
}

void AdjacencyList::printGraph() {
	for (int i = 0; i < vertices; i++) {
		cout << i << ": ";
		for (EdgeData edge : nList[i]) {
			cout << "(" << edge.v << " : " << edge.remainingFlow << " )";
		}
		cout << endl;
	}
}