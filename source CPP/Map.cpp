// Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.

/*
TODO dla pierwszej czesci:
1. Stworzyc brakujace klasy
2. Wczytywanie nazw Node'ow zamiast indeksow
3. Generowanie grafu na podstawie podanych Node'ow
4. Browary przemnazajace przechodzaca wartosc
*/


//#include "Node.h"
//#include "Field.h"
//#include "Brewery.h"
//#include "Pub.h"


#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

//#define INF INT_MAX

// BFS for finding the augumenting path in the current state of the graph
bool bfs(vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent) {
    int V = residualGraph.size();
    vector<bool> visited(V, false);
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < V; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                parent[v] = u;
                visited[v] = true;
                q.push(v);
                if (v == sink) return true; // found a path to sink
            }
        }
    }
    return false;
}


int fordFulkerson(vector<vector<int>> graph, int source, int sink) {
    int V = graph.size();
    //vector<vector<int>> residualGraph = graph; // copy the graph to use as the residual graph
    vector<int> parent(V);
    int maxFlow = 0;

    while (bfs(graph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        vector<int> path;

        // find the bottleneck (min flow in path)
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, graph[u][v]);
        }

        // update the residual graph
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            graph[u][v] -= pathFlow;
            graph[v][u] += pathFlow;
            path.push_back(v);
        }
        path.push_back(source);
        reverse(path.begin(), path.end());

        // display the found augumenting path
        cout << "Sciezka powiekszajaca: ";
        for (int node : path) {
            cout << node << " ";
        }
        cout << "| Przeplyw: " << pathFlow << endl;

        maxFlow += pathFlow;
    }
    return maxFlow;
}

int main() {
    ifstream inputFile("daneZwagami.txt");
    if (!inputFile) {
        cerr << "Blad otwarcia pliku!" << endl;
        return 1;
    }

    int vertices, edges;
    inputFile >> vertices >> edges;
    vector<vector<int>> graph(vertices, vector<int>(vertices, 0));

    int u, v, capacity;
    while (inputFile >> u >> v >> capacity) {
        graph[u][v] = capacity;
    }
    inputFile.close();

    int source = 0, sink = vertices - 1;

    cout << "Maksymalny przeplyw: " << fordFulkerson(graph, source, sink) << endl;
    return 0;
}