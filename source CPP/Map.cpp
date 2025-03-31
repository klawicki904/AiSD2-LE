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


#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
//#include <algorithm>
//#include <iomanip>

using namespace std;

struct edgeData
{
    int to;
    //int maxFlow;
    int remainingFlow;
    //int backwardsFlow;
};

int main()
{
    ifstream plik("daneZwagami.txt");

    int vertices;
    int newEdge1, newEdge2, maxFlow;
    string line;

    if (!(plik >> vertices >> newEdge1))
    {
        return -1;
    }
    //vertices++;

    vector<vector<edgeData>> nList(vertices);

    while (plik >> newEdge1 >> newEdge2 >> maxFlow)
    {
        edgeData edgeDataTemp;
        edgeDataTemp.to = newEdge2;
        //edgeDataTemp.maxFlow = maxFlow;
        edgeDataTemp.remainingFlow = maxFlow;
        //edgeDataTemp.backwardsFlow = 0;

        nList[newEdge1].push_back(edgeDataTemp);

        edgeDataTemp.to = newEdge1;
        edgeDataTemp.remainingFlow = 0;
        nList[newEdge2].push_back(edgeDataTemp);
    }

    int finish = nList.size() - 1;
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
        for (int i = 0; i < vertices - 1; i++)
        {
            cout << i << ": \t";
            for (unsigned int j = 0; j < nList[i].size(); j++)
            {
                cout << "( " << nList[i][j].to << ", " << nList[i][j].remainingFlow << " )\t";
            }
            cout << endl;
        }


        int start = 0;
        vector<bool> visited(nList.size(), false);
        vector<int> parents(nList.size(), -1);
        vector<int> distances(nList.size(), INT_MAX);
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
            for (int i = 0; i < nList[u].size(); i++)
            {
                v = nList[u][i].to;
                if (visited[v] == false && nList[u][i].remainingFlow > 0)
                {
                    visited[v] = true;
                    parents[v] = u;
                    distances[v] = min(distances[u], nList[u][i].remainingFlow);
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

        finish = nList.size() - 1;
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
            for (int j = 0; j < nList[v].size(); j++)
            {
                if (nList[v][j].to == shortestPath[i + 1])
                {
                    nList[v][j].remainingFlow -= distances[finish];
                    // find the opposite direction (b->a) and add flow
                    for (j = 0; j < nList[shortestPath[i + 1]].size(); j++)
                    {
                        if (nList[shortestPath[i + 1]][j].to == shortestPath[i])
                        {
                            nList[shortestPath[i + 1]][j].remainingFlow += distances[finish];
                        }
                        break; // found the edge b->a so stop searching
                    }
                    break; // found the edge a->b so stop searching
                }
            }
        }
    }

    cout << "Max Flow In Whole Path: " << maxFlowInPath << endl;

}