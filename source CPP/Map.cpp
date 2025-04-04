// Ten plik zawiera funkcjê „main”. W nim rozpoczyna siê i koñczy wykonywanie programu.

// algorytm Forda-Fulkersona u¿ywa³ macierzy s¹siedztwa, bo 
// z list¹ s¹siedztwa by³ problem przy przechodzeniu w drug¹ stronê
// problem zosta³ naprawiony, teraz algorytm u¿ywa listy s¹siedztwa

/*
Do zrobienia dla ca³ego projektu:
1. Generator grafów (plik z po³¹czeniami) do testowania algorytmu.
2. GUI
3. Dokumentacja

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

double beerConvertsionRate = 0.5; // jeszcze nie u¿yte
// mo¿emy zapisaæ ka¿dy typ w oddzielnej klasie, ale bêd¹ przechowywaæ co najwy¿ej swoje po³o¿enie i jedn¹ zmienn¹
// (jeszcze nie u¿yte) rodzaj wêz³a (¿adne (Ÿród³o lub ujœcie), pole, browarnia, karczma)
enum NodeType
{
    none,
    field,
    brewery,
    pub
};
// (jeszcze nie u¿yte) struktura wektor do przechowania po³o¿enia wierzcho³ka
struct Vector2
{
    int x = 0;
    int y = 0;
};

// struktura trzymaj¹ca informacje o drodze (strza³ce) miêdzy wêz³ami
struct Road
{
    NodeType nodeType = none; // jeszcze nie u¿ywane
    Vector2 position; // jeszcze nie u¿ywane
    int to = -1; // indeks wierzcho³ka, do którego droga prowadzi
    double remainingFlow = 0; // pozosta³y przep³yw
};

void bfsFlow(vector<vector<Road>> &aList, vector<int> &parents, vector<double> &flows, int start = 0)
{
    vector<bool> visited(aList.size(), false); // odwiedzone 
    queue<int> Q; // kolejka
    //parents[start] = -2;

    visited[start] = 1;
    Q.push(start); // na pocz¹tek dodaje wêze³ startowy do kolejki
    int v = 0;
    while (!Q.empty())
    {
        int u = Q.front(); // zapisuje element na pocz¹tku kolejki
        Q.pop(); // usuwa element z pocz¹tku kolejki
        for (int i = 0; i < aList[u].size(); i++) // dla ka¿dej drogi z u
        {
            v = aList[u][i].to;
            // dodaje wêze³ v do kolejki jeœli nieodwiedzony i ma dostêpny przep³yw
            if (visited[v] == false && aList[u][i].remainingFlow > 0)
            {   // po dodaniu:
                visited[v] = true; // wêze³ ma status odwiedzony
                parents[v] = u; // zapisuje u jako poprzednik v
                flows[v] = min(flows[u], aList[u][i].remainingFlow); // aktualizuje maksymalny przep³yw dla wêz³a
                Q.push(v); // dodaje wêze³ v do kolejki
            }
        }
    }
}

double fordFulkerson(vector<vector<Road>>& aList)
{
    double totalFlow = 0;
    int vertices = aList.size();
    while (true) // wykonuje, a¿ nie znajdzie œcie¿ki powiêkszaj¹cej
    {
        /// wyswietla stan listy sasiedztwa
        cout << "\aLista sasiedztwa: z: ( do, pozostaly ):\n";
        for (int i = 0; i < vertices - 1; i++)
        {
            cout << i << ": \t";
            for (unsigned int j = 0; j < aList[i].size(); j++)
            {
                cout << "( " << aList[i][j].to << ", " << aList[i][j].remainingFlow << " )\t";
            }
            cout << endl;
        }
        // BFS
        int start = 0; // indeks wêz³a startowego
        vector<int> parents(vertices, -1); // poprzedniki, na pocz¹tku wszystkie -1
        vector<double> flows(vertices, INT_MAX); // maksymalny przep³yw do ka¿dego wêz³a

        bfsFlow(aList, parents, flows); // BFS

        // wypisuje tablicê poprzedników
        cout << "\nparents:\n";
        for (int i = 0; i < parents.size(); i++)
        {
            cout << i << ": " << parents[i] << "\n";
        }
        // wypisuje tablicê przep³ywów
        cout << "\nflows:\n";
        for (int i = 0; i < flows.size(); i++)
        {
            cout << i << ": " << flows[i] << "\n";
        }

        int finish = vertices - 1;
        /// nie znaleziono sciezki do ujœcia, koniec algorytmu FF
        if (parents[finish] == -1) break;

        // zapisz œcie¿kê powiêkszaj¹c¹ na podstawie tablicy poprzedników
        vector<int> shortestPath;
        cout << "\nShortest path:\n";
        shortestPath.push_back(finish);
        do
        {
            finish = parents[finish];
            shortestPath.push_back(finish);
        } while (finish != start);

        finish = vertices - 1;
        reverse(shortestPath.begin(), shortestPath.end());

        // wymieñ wierzcho³ki tworz¹ce œcie¿kê powiêkszaj¹c¹
        for (int i = 0; i < shortestPath.size(); i++)
        {
            cout << shortestPath[i] << ", ";
        }
        cout << endl;

        cout << "Max Flow In This Path: " << flows[finish] << endl;
        totalFlow += flows[finish];

        // zaktualizuj przep³yw w liœcie s¹siedztwa
        int v;
        for (int i = 0; i < shortestPath.size() - 1; i++) // dla ka¿dego wêz³a w œcie¿ce powiêkszaj¹cej (bez ostatniego)
        {
            v = shortestPath[i];
            for (int j = 0; j < aList[v].size(); j++) // przeszukaj jego listê s¹siedztwa
            {
                if (aList[v][j].to == shortestPath[i + 1]) // ¿eby znaleŸæ ten, który jest nastêpny w œcie¿ce powiêkszaj¹cej
                {
                    aList[v][j].remainingFlow -= flows[finish]; // odejmij przep³yw
                    // zaktualizuj przep³yw w przeciwnym kierunku
                    for (j = 0; j < aList[shortestPath[i + 1]].size(); j++) // znajdŸ strza³kê w drug¹ stronê
                    {
                        if (aList[shortestPath[i + 1]][j].to == shortestPath[i])
                        {
                            aList[shortestPath[i + 1]][j].remainingFlow += flows[finish]; // dodaj przep³yw w przeciwnym kierunku
                        }
                        break; // droga b->a znaleziona, nie szukaj dalej
                    }
                    break; // droga a->b znaleziona, nie szukaj dalej
                }
            }
        }
    }
    return totalFlow;
}

int main()
{
    // 1a. Przerobiæ ten plik tak, ¿eby wczytywa³ z pliku node'y zamiast po³¹czeñ




    ifstream plik("daneZwagami.txt");
    //zmienne do wczytania pliku z krawêdziami
    int vertices, newEdge1, newEdge2, maxFlow;
    //string line;
    // wczyta pierwszy wiersz (liczba wêz³ów i krawêdzi)
    // wczytywanie krawêdzi jest niepotrzebne, ¿eby ³atwiej by³o przekopiowaæ do visualgo
    if (!(plik >> vertices >> newEdge1))
    {
        return -1;
    }
    // tworzy listê s¹siedztwa
    vector<vector<Road>> aList(vertices);

    while (plik >> newEdge1 >> newEdge2 >> maxFlow)
    {
        // tworzy tymczasow¹ strukturê, uzupe³nia danymi z pliku i dodaje do listy s¹siedztwa
        Road roadTemp;
        roadTemp.to = newEdge2;
        roadTemp.remainingFlow = maxFlow;
        aList[newEdge1].push_back(roadTemp);


        // dodaje do listy s¹siedztwa po³¹czenie w przeciwnym kierunku z zerowym przep³ywem
        // TODO: do this only when path doesn't already exist
        roadTemp.to = newEdge1;
        roadTemp.remainingFlow = 0;
        aList[newEdge2].push_back(roadTemp);
    }

    cout << "Max Flow In Whole Path: " << fordFulkerson(aList) << endl;

    return 0;
}