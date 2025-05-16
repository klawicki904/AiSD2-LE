#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <functional>
#include "Matrix.h"
#include "Node.h"
#include "EdgeData.h"
#include <vector>
#include <fstream>
#include <queue>
#include <limits>


using namespace std;

class Matrix
{
public:
    int vertices; //Rozmiar macierzy
    vector<vector<EdgeData>> tab; // Macierz s?siedztwa
    vector<Node> listVertives; // Zawiera informacje o wierzcho?kach

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

    void dijkstra(int source);

    bool dijkstraModify(int source,  int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf);

    bool BellmanFord(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf);

    double BusackerGowen(double const maxFlow,
        bool (Matrix::* shortestPathFunc)(int, int, double&, vector<int>&, const vector<vector<EdgeData>>&));

    //Klasyczna metoda wyliczaj?ca maksymalny przep?yw
    double edmonsKarp();

    void maxFlowMinCost();

    //Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
    bool bfs(int x, const vector<vector<EdgeData>>& graf, int t, vector<int>& f);


    ////Ford Fulkerson dla sieci w ktorej interesuja nas sciezki powiekszajace s-> ... -> t 
    // (Gdzie gdzies po drodze jest wierzcholek ktory jest browarem)
    double edmonsKarp2();

    bool bfs2(int x, const vector<vector<EdgeData>>& graf, int t, vector<int>& f);
};

#endif	

