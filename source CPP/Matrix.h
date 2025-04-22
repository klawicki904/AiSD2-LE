#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include "Matrix.h"
#include "Node.h"
#include <vector>
#include <fstream>
#include <queue>

using namespace std;

class Matrix
{
public:
	int vertices; //Rozmiar macierzy
	vector<vector<double>> tab; // Macierz s?siedztwa
    vector<Node> listVertives; // Zawiera informacje o wierzcho?kach

    // Konstruktory
    Matrix();
    Matrix(int n);

    // Metoda do inicjalizacji pól klasy
    void init(int n);

    //Dodaje krawedz
    void addEdge(int u, int v, double weight);

    //Dodaje info o Node
    void addNode(const Node& node);

    //Wypisuje macierz
    void printGraph() const;
    
    //Czyta dane z pliku i inicjalizuje macierz
    bool readFileToGraph(string fileName);

    //Klasyczna metoda wyliczaj?ca maksymalny przep?yw
    double fordFulkerson();

    //Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
    bool bfs(int x, const vector<vector<double>>& graf, int t, vector<int>& f);

    //W trakcie pracy. Jeszcze nie dziala prawidlowo ! 
    ////Ford Fulkerson dla sieci w ktorej interesuja nas sciezki powiekszajace s-> ... -> t 
    // (Gdzie gdzies po drodze jest wierzcholek ktory jest browarem)
    double fordFulkerson2();

    //W trakcie pracy. Jeszcze nie dziala prawidlowo !
    //Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t (ale przechodzi przez browar!) to zwraca true. Aktulizuje tez tablice ojcow
    bool bfs2(int s, const vector<vector<double>>& graf, int t, vector<int>& f, const vector<Node> type);
};

#endif	

