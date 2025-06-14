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
    int source; // ?r�d?o
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

    //Czyta dane z pliku i inicjalizuje macierz
    bool readFileToGraph(string fileName);

    // Wczytywanie pliku wejsciowego przed "cwiartki update"
    //bool readFileToGraph2(string fileName);

    // Remaster wczytywania autorstwa JK
    bool readFileToGraph3(string fileName);

    //Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
    bool bfs(int x, const vector<vector<EdgeData>>& graf, int t, vector<int>& f);

    //algorytm znajdowania najkrotszych sciezek. Zmodyfikowany pod nasz problem
    bool dijkstraModify(int source,  int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf);

    //algorytm znajdowania najkrotszych sciezek.
    bool BellmanFord(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf);

    //Algorytm Busackera_Gowena wyznacza najta?szy przep?yw w sieci dla docelowego przep?ywu F.
    // Nie tworze w nim sieci residualnej. Dzia?a na orginale. Zatem uruchomienie dwukrotnie tej metody nie da dobrych wynikow
    // Nalezalo by po wywolaniu tej metody. naprawic siec (np. ponownie wczytac z pliku) by uruchomic j? po raz kolejny
    double BusackerGowen2(double const maxFlow, int s, int t, vector<Path>& roads,
        bool (Matrix::* shortestPathFunc)(int, int, double&, vector<int>&, const vector<vector<EdgeData>>&));

    //Klasyczna metoda wyliczaj?ca maksymalny przep?yw bez przekazywania argumentow
    double edmondsKarp();

    // Klasyczny algorytm Edmondsa-Karpa. Z podaniem argumentow
    double edmondsKarpClassic(int s, vector<vector<EdgeData>>& graf, int t, vector<int>& f);

    // rozwiazanie problemu. tj liczenie maksymalnego przeplywu i minimalnego kosztu.
    void maxFlowMinCost();

    // rozwiazanie problemu. tj liczenie maksymalnego przeplywu i minimalnego kosztu. Dla problemu s->Browary->Puby
    double maxFlowMinCost2(string outputPath);


    //Stary "edmonsKarp2()". Liczy maksymalny przeplyw dla naszego problemu.
    pair<double, double> maxFlowAlgorithm();

    //wypisuje lub zapisuje do pliku
    void printToFileSolution(double maxFlow, vector<Path> combined);

    //void printToFileSolution(double maxFlow);
    void printToFileSolution2(double resultFlow, vector<Path>  combined, string outputPath);

      
  
};

#endif	

