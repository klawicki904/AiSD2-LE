// Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.

// algorytm Forda-Fulkersona u¿ywa³ macierzy s¹siedztwa, bo 
// z list¹ s¹siedztwa by³ problem przy przechodzeniu w drug¹ stronê
// problem zosta³ naprawiony, teraz algorytm u¿ywa listy s¹siedztwa

/*
Do zrobienia dla ca³ego projektu:
1. Generator grafów (plik z po³¹czeniami) do testowania algorytmu.
2. GUI

Do zrobienia dla pierwszej czêœci:
1a. Przerobiæ ten plik tak, ¿eby wczytywa³ z pliku node'y zamiast po³¹czeñ
1b. Przerobiæ ten plik tak, ¿eby generowa³ plik z po³¹czeniami na podstawie wczytanych node'ów
2. Browary przemna¿aj¹ce przechodz¹c¹ wartoœæ


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

double beerConvertsionRate = 0.5;
enum NodeType
{
    none,
    field,
    brewery,
    pub
};

struct Vector2
{
    int x;
    int y;
};

struct edgeData
{
    NodeType nodeType = none;
    Vector2 position;
    int to;
    double remainingFlow;
};

int main()
{
    // 1a. Przerobiæ ten plik tak, ¿eby wczytywa³ z pliku node'y zamiast po³¹czeñ




    ifstream plik("daneZwagami.txt");

    int vertices;
    int newEdge1, newEdge2, maxFlow;
    string line;

    if (!(plik >> vertices >> newEdge1))
    {
        return -1;
    }

    vector<vector<edgeData>> nList(vertices);

    int lastBrewery; // index of last brewery to multiply max flows of the ones after

    while (plik >> newEdge1 >> newEdge2 >> maxFlow)
    {
        // 2. Browary przemna¿aj¹ce przechodz¹c¹ wartoœæ
        edgeData edgeDataTemp;
        edgeDataTemp.to = newEdge2;
        edgeDataTemp.remainingFlow = maxFlow;
        nList[newEdge1].push_back(edgeDataTemp);

        // TODO: do this only when path doesn't already exist
        edgeDataTemp.to = newEdge1;
        edgeDataTemp.remainingFlow = 0;
        nList[newEdge2].push_back(edgeDataTemp); // create a path in the opposite direction with no flow
    }

    int finish = nList.size() - 1;
    double maxFlowInPath = 0;
    /// BFS:
    /// tablica poprzednikow: -2 dla startowego, -1 dla pozostalych, obecny wierzcholek dla dodanych do kolejki
    /// jesli dostepne 0 przeplywu, nie przechodzimy
    /// szukamy najkrotszej drogi w sensie ilosci wierzcholkow
    /// robimy tak dlugo, az nie znajdziemy sciezek powiekszajacych
    while (true)
    {
        /// wyswietl stan listy sasiedztwa
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
        vector<double> distances(nList.size(), INT_MAX);
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
                    if (nList[v][j].nodeType == brewery)
                    {
                        nList[v][j].remainingFlow -= distances[finish] * beerConvertsionRate;
                    }
                    else
                    {
                        nList[v][j].remainingFlow -= distances[finish];
                    }
                    // find the opposite direction (b->a) and add flow
                    for (j = 0; j < nList[shortestPath[i + 1]].size(); j++)
                    {
                        if (nList[shortestPath[i + 1]][j].to == shortestPath[i])
                        {
                            if (nList[v][j].nodeType == brewery)
                            {
                                nList[shortestPath[i + 1]][j].remainingFlow += distances[finish] * beerConvertsionRate;
                            }
                            else
                            {
                                nList[shortestPath[i + 1]][j].remainingFlow += distances[finish];
                            }
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