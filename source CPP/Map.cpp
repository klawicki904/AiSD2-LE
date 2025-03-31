// Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.

// algorytm Forda-Fulkersona u¿ywa macierzy s¹siedztwa, bo 
// z list¹ s¹siedztwa by³ problem przy przechodzeniu w drug¹ stronê

/*
TODO dla pierwszej czêœci:
1. Stworzyæ brakuj¹ce klasy
2. Wczytywanie nazw Node'ów zamiast indeksów
3. Generowanie grafu na podstawie podanych Node'ów
4. Browary przemnazaj¹ce przechodzac¹ wartoœæ
*/


//#include "Node.h"
//#include "Field.h"
//#include "Brewery.h"
//#include "Pub.h"


// Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.

// algorytm Forda-Fulkersona u¿ywa macierzy s¹siedztwa, bo 
// z list¹ s¹siedztwa by³ problem przy przechodzeniu w drug¹ stronê

/*
TODO dla pierwszej czêœci:
1. Stworzyæ brakuj¹ce klasy
2. Wczytywanie nazw Node'ów zamiast indeksów
3. Generowanie grafu na podstawie podanych Node'ów
4. Browary przemnazaj¹ce przechodzac¹ wartoœæ
*/


//#include "Node.h"
//#include "Field.h"
//#include "Brewery.h"
//#include "Pub.h"
#include "EdgeData.h" //dane krawedzi
#include "AdjacencyList.h" //lista sasiedztwa

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
//#include <algorithm>
//#include <iomanip>


using namespace std;


int main()
{
    AdjacencyList nList;
    if (!nList.readFileToGraph("daneZwagami.txt")) {
        return -1;
    }

    //nList.printGraph();


    int finish = nList.vertices - 1;
    int maxFlowInPath = 0;
    /// BFS:
    /// tablica poprzednikow: -2 dla startowego, -1 dla pozostalych, obecny wierzcholek dla dodanych do kolejki
    /// jesli dostepne 0 przeplywu, nie przechodzimy
    /// szukamy najkrotszej drogi w sensie ilosci wierzcholkow
    /// robimy tak dlugo, az nie znajdziemy sciezek powiekszajacych
    while (true)
    {
        /// lista sasiedztwa
        cout << "\nLista sasiedztwa: z: ( do, pozostaly ):\n";
        for (int i = 0; i < nList.vertices - 1; i++)
        {
            cout << i << ": \t";
            //for (unsigned int j = 0; j < nList[i].size(); j++)
            for (EdgeData j : nList.nList[i])
            {
                cout << "( " << j.v << ", " << j.remainingFlow << " )\t";
            }
            cout << endl;
        }
    

        int start = 0;
        vector<bool> visited(nList.vertices, false);
        vector<int> parents(nList.vertices, -1);
        vector<int> distances(nList.vertices, INT_MAX);
        queue<int> Q;
        parents[start] = -2;

        visited[start] = 1;
        Q.push(start);
        int v = 0;
        while (!Q.empty())
        {
            int u = Q.front();
            Q.pop();
            //cout<< Q.front()<<" ";
            for (int i = 0; i < nList.nList[u].size(); i++)
            {
                v = nList.nList[u][i].v;
                if (visited[v] == false && nList.nList[u][i].remainingFlow > 0)
                {
                    visited[v] = true;
                    parents[v] = u;
                    distances[v] = min(distances[u], nList.nList[u][i].remainingFlow);
                    Q.push(v);
                }
            }

            //visited[u] = 2;
        }

        cout << "\nparents:\n";
        for (int i = 0; i < parents.size(); i++)
        {
            cout << i << ": " << parents[i] << "\n";
        }

        cout << "\ndistances:\n";
        for (int i = 0; i < distances.size(); i++)
        {
            cout << i << ": " << distances[i] << "\n";
        }

        /// nie znaleziono sciezki do ujscia
        if (parents[finish] == -1) break;

        vector<int> shortestPath;
        //maxFlowInPath = distances[nList.size() - 1];

        cout << "\nShortest path:\n";

        //cout << finish;
        shortestPath.push_back(finish);
        do
        {
            finish = parents[finish];
            //cout << " -> " << finish;
            shortestPath.push_back(finish);
        } while (finish != start);

        finish = nList.vertices - 1; //bylo zmieniane
        reverse(shortestPath.begin(), shortestPath.end());

        //cout << endl;
        for (int i = 0; i < shortestPath.size(); i++)
        {
            cout << shortestPath[i] << ", ";
        }
        cout << endl;

        cout << "Max Flow In This Path: " << distances[finish] << endl;
        maxFlowInPath += distances[finish];

        // update flow in the found path
        for (int i = 0; i < shortestPath.size() - 1; i++)
        {
            v = shortestPath[i];
            // find the edge a->b from shortestPath in adjacency list and subtract flow
            for (int j = 0; j < nList.nList[v].size(); j++)
            {
                if (nList.nList[v][j].v == shortestPath[i + 1])
                {
                    nList.nList[v][j].remainingFlow -= distances[finish];
                    // find the opposite direction (b->a) and add flow
                    for (j = 0; j < nList.nList[shortestPath[i + 1]].size(); j++)
                    {
                        if (nList.nList[shortestPath[i + 1]][j].v == shortestPath[i])
                        {
                            nList.nList[shortestPath[i + 1]][j].remainingFlow += distances[finish];
                        }
                        break; // found the edge b->a so stop searching
                    }
                    break; // found the edge a->b so stop searching
                }
            }
        }
    }

    cout << "Max Flow In Whole Path: " << maxFlowInPath << endl;
    return 0;
}