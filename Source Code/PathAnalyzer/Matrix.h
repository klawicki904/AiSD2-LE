#ifndef MATRIX_H
#define MATRIX_H


#include <iostream>
#include <functional>
#include "Matrix.h"
#include "Node.h"
#include "EdgeData.h"
#include "Quarter.h"

#include "printNodesRoads.h"
#include "printRoutes.h"

#include <vector>
#include <fstream>
#include <queue>
#include <limits>
#include "Path.h"
#include <unordered_map> //do wypisania wynikow
#include <locale> 
#include <codecvt>
#include <io.h>
#include <fcntl.h>
#define NOMINMAX
#include <windows.h>
#include <algorithm>

using namespace std;

class Matrix
{
public:
    int vertices; // Rozmiar macierzy
    int source; // ?ród?o
    int target; // Uj?cie
    vector<vector<EdgeData>> tab; // Macierz s?siedztwa
    vector<Node> listVertices; // Zawiera informacje o wierzcho?kach
    vector<Quarter> initialQuarters;

    // Konstruktory
    Matrix();
    Matrix(int n);

    // Metoda do inicjalizacji p?l klasy
    void init(int n);

    //Dodaje krawedz
    void addEdge(int u, int v, double weight, double cost);

    //Dodaje info o Node
    void addNode(const Node& node);

    //Wypisuje macierz
    void printGraph() const;

    // Remaster wczytywania autorstwa JK
    bool readFileToGraph3(string fileName);

    //Tworzy siec residualna. Pod nasz problem
    void createResidualNet(vector<vector<EdgeData>>& graf , const vector<Node>& listNodes);

    //Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
    bool bfs(int x, const vector<vector<EdgeData>>& graf, int t, vector<int>& f);

    //algorytm znajdowania najkrotszych sciezek. Zmodyfikowany pod nasz problem
    bool dijkstraModify(int source,  int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf);

    //algorytm znajdowania najkrotszych sciezek.
    bool BellmanFord(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf);

    // Wyznacza minCostMaxFlow z uwzglednieniem konwersji. Do szukania najta?szych ?cie?ek: Dijkstra z potencja?ami (algorytm Johnsona)
    double BusackerGowen3(double const maxFlow, int s, int t, vector<Path>& roads, double konwersja, int midLayer);

    //Klasyczna metoda wyliczaj?ca maksymalny przep?yw bez przekazywania argumentow
    double edmondsKarp();

    // Dijkstra z kosztami zredukowanymi przez potencja?y
    bool dijkstraWithPotentials(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf, const vector<double>& pot) const;

    // Bellman-Ford: wyznacza potencja?y dla macierzy s?siedztwa
    // Potencja?y: pot[i] to koszt najta?szej ?cie?ki z wierzcho?ka t do i
    void computePotentials(vector<double>& pot) const;

    // Klasyczny algorytm Edmondsa-Karpa. Z podaniem argumentow
    double edmondsKarpClassic(int s, vector<vector<EdgeData>>& graf, int t, vector<int>& f);

    // rozwiazanie problemu. tj liczenie maksymalnego przeplywu i minimalnego kosztu. Dla problemu s->Browary->Puby
    double maxFlowMinCost3(string outputPath);

    // Wypisuje rozwiazanie
    void printToFileSolution2(double resultFlow, vector<Path>  combined, string outputPath);

    // algorytm Edmondsa-Karpa z uwzglednienim konwersji
    double edmondsKarpClassicWithConversion(int s, int t, double conversion, int midLayer);

      
};

#endif	

