// Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.

/*
TODO dla pierwszej czesci:
0. naprawic algorytm Forda-Fulkersona (brakuje dodawania przeplywu w przeciwnym kierunku)
1. Stworzyc brakujace klasy
2. Wczytywanie nazw Node'ow zamiast indeksow
3. Generowanie Road'ow na podstawie podanych Node'ow
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
#include <algorithm>
#include <iomanip>

using namespace std;

struct Road
{
    int to;
    int remainingFlow;
};

int main()
{
    //Field testField = Field(1, 2, 1.3);
    //cout << testField.GetX() << ", " << testField.GetY() << ", " << testField.GetCapacity() << endl;

    ifstream plik("daneZwagami.txt");

    int vertices;
    int newEdge1, newEdge2, maxFlow;
    string line;

    if (!(plik >> vertices >> newEdge1))
    {
        return -1;
    }
    //vertices++;

    vector<vector<Road>> nList(vertices);

    while (plik >> newEdge1 >> newEdge2 >> maxFlow)
    {
        Road edgeDataTemp;
        edgeDataTemp.to = newEdge2;
        //edgeDataTemp.maxFlow = maxFlow;
        edgeDataTemp.remainingFlow = maxFlow;
        //edgeDataTemp.backwardsFlow = 0;

        nList[newEdge1].push_back(edgeDataTemp);
    }

    size_t finish = nList.size() - 1;
    int maxFlowInPath = 0;
    /// BFS:
    /// tablica poprzednikow: -2 dla startowego, -1 dla pozostalych, obecny wierzcholek dla dodanych do kolejki
    /// jesli dostepne 0 przeplywu, nie przechodzimy
    /// szukamy najkrotszej drogi w sensie ilosci wierzcholkow
    /// robimy tak dlugo, az nie znajdziemy sciezek powiekszajacych
    while (true)
    {
        /// lista sasiedztwa
        cout << "\nLista sasiedztwa: z: ( do, pozostaly):\n";
        for (int i = 0; i < vertices - 1; i++)
        {
            cout << i << ": \t";
            for (unsigned int j = 0; j < nList[i].size(); j++)
            {
                //cout << "( " << nList[i][j].to << ", " << nList[i][j].maxFlow << ", " << nList[i][j].remainingFlow << ", " << nList[i][j].backwardsFlow << " )\t";
                cout << "( " << nList[i][j].to << ", " << nList[i][j].remainingFlow << " )\t";
            }
            cout << endl;
        }


        int start = 0;
        vector<int> visited(nList.size());
        vector<int> parents(nList.size());
        vector<int> distances(nList.size());
        queue<int> Q;
        for (int i = 0; i < parents.size(); i++)
        {
            parents[i] = -1;
            distances[i] = INT_MAX;
        }
        parents[start] = -2;

        visited[start] = 1;
        Q.push(start);
        int v = 0;
        while (!Q.empty())
        {
            int u = Q.front();
            //cout<< Q.front()<<" ";
            for (int i = 0; i < nList[u].size(); i++)
            {
                v = nList[u][i].to;
                if (visited[v] == 0 && nList[u][i].remainingFlow > 0)
                {
                    visited[v] = 1;
                    parents[v] = u;
                    distances[v] = min(distances[u], nList[u][i].remainingFlow);
                    Q.push(v);
                }
            }
            Q.pop();
            visited[u] = 2;
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

        for (int i = 0; i < shortestPath.size() - 1; i++)
        {
            v = shortestPath[i];

            for (int j = 0; j < nList[v].size(); j++)
            {
                if (nList[v][j].to == shortestPath[i + 1])
                {
                    nList[v][j].remainingFlow -= distances[finish];
                }
            }
        }
    }

    cout << "Max Flow In Whole Path: " << maxFlowInPath << endl;

}