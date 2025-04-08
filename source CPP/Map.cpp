// Ten plik zawiera funkcj� �main�. W nim rozpoczyna si� i ko�czy wykonywanie programu.

// algorytm Forda-Fulkersona u�ywa� macierzy s�siedztwa, bo 
// z list� s�siedztwa by� problem przy przechodzeniu w drug� stron�
// problem zosta� naprawiony, teraz algorytm u�ywa listy s�siedztwa

/*
Do zrobienia:
1. Dokumentacja (dokument tekstowy i arkusz/harmonogram)

2. Interface konsolowy - program, kt�ry b�dzie:
- wczytywa� z pliku i zapisywa� do pliku informacje o node'ach i drogach;
- generowa� plik ze �cie�kami, kt�ry b�dzie od razu dzia�a� dla algorytmu poszukuj�cego
- przechowywa� dane po wczytaniu
- pozwala� na wy�wietlanie danych w konsoli
- pozwala� na modyfikacj� danych w konsoli
- dba� o prawid�owo�� wprowadzonych danych
przyk�adowy plik:
0.5 (konwersja j�czmienia na piwo)
FIELDS
1 0.1 1.2 FIELD 30
BREWERIES
2 3.5 2.2 BREWERY 20
3 4.5 3.2 BREWERY 15
4 5.5 4.2 BREWERY 10
PUBS
5 5.6 2.7 PUB
6 6.7 3.8 PUB
ROADS
4 (ilo�� wierzcho�k�w) 8 (ilo�� po��cze�)
1 2 30.0
1 3 20.0
1 4 30.0
2 5 7.0
2 6 3.2
3 5 8.2
4 5 8.5
4 6 6.9

3. Modyfikacja algorytmu przep�ywu dla problemu projektu (ten plik):
3a. Pole, przez kt�re przechodzi �cie�ka powi�kszaj�ca musi aktualizowa� maksymalny przep�yw dla ka�dej kraw�dzi, kt�ra z niego wychodzi
np. je�li pole ma 30 zbo�a i wychodz� z niego 2 strza�ki po 30, po przeprowadzeniu 25 zbo�a przez �cie�k� trzeba ustawi� wszystkie strza�ki na max(obecny, 30-25)
3b. Browarnia, przez kt�r� przechodzi �cie�ka powi�kszaj�ca musi aktualizowa� przep�yw dla ka�dej kraw�dzi, kt�ra do niego prowadzi, �eby nie dosz�o do przekroczenia pojemno�ci browarni

4. Browary przemna�aj�ce przechodz�c� warto��

5. GUI (b�dzie dba�o o prawid�owy przep�yw mi�dzy polami i browarami)


?. Generator graf�w (plik z po��czeniami) do testowania algorytmu:
Wej�cie: liczba ca�kowita - ilo�� node'�w
Wyj�cie: plik tekstowy mo�liwy do przetworzenia przez algorytm

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

double beerConvertsionRate = 0.5; // jeszcze nie u�yte
// mo�emy zapisa� ka�dy typ w oddzielnej klasie, ale b�d� przechowywa� co najwy�ej swoje po�o�enie i jedn� zmienn�
// (jeszcze nie u�yte) rodzaj w�z�a (�adne (�r�d�o lub uj�cie), pole, browarnia, karczma)
enum NodeType
{
    none,
    field,
    brewery,
    pub
};
// (jeszcze nie u�yte) struktura wektor do przechowania po�o�enia wierzcho�ka
struct Vector2
{
    int x = 0;
    int y = 0;
};

// struktura trzymaj�ca informacje o drodze (strza�ce) mi�dzy w�z�ami
struct Road
{
    NodeType nodeType = none; // jeszcze nie u�ywane
    Vector2 position; // jeszcze nie u�ywane
    int to = -1; // indeks wierzcho�ka, do kt�rego droga prowadzi
    double remainingFlow = 0; // pozosta�y przep�yw
};

void bfsFlow(vector<vector<Road>> &aList, vector<int> &parents, vector<double> &flows, int start = 0)
{
    vector<bool> visited(aList.size(), false); // odwiedzone 
    queue<int> Q; // kolejka
    //parents[start] = -2;

    visited[start] = 1;
    Q.push(start); // na pocz�tek dodaje w�ze� startowy do kolejki
    int v = 0;
    while (!Q.empty())
    {
        int u = Q.front(); // zapisuje element na pocz�tku kolejki
        Q.pop(); // usuwa element z pocz�tku kolejki
        for (int i = 0; i < aList[u].size(); i++) // dla ka�dej drogi z u
        {
            v = aList[u][i].to;
            // dodaje w�ze� v do kolejki je�li nieodwiedzony i ma dost�pny przep�yw
            if (visited[v] == false && aList[u][i].remainingFlow > 0)
            {   // po dodaniu:
                visited[v] = true; // w�ze� ma status odwiedzony
                parents[v] = u; // zapisuje u jako poprzednik v
                flows[v] = min(flows[u], aList[u][i].remainingFlow); // aktualizuje maksymalny przep�yw dla w�z�a
                Q.push(v); // dodaje w�ze� v do kolejki
            }
        }
    }
}

double fordFulkerson(vector<vector<Road>>& aList)
{
    double totalFlow = 0;
    int vertices = aList.size();
    while (true) // wykonuje, a� nie znajdzie �cie�ki powi�kszaj�cej
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
        int start = 0; // indeks w�z�a startowego
        vector<int> parents(vertices, -1); // poprzedniki, na pocz�tku wszystkie -1
        vector<double> flows(vertices, INT_MAX); // maksymalny przep�yw do ka�dego w�z�a

        bfsFlow(aList, parents, flows); // BFS

        // wypisuje tablic� poprzednik�w
        cout << "\nparents:\n";
        for (int i = 0; i < parents.size(); i++)
        {
            cout << i << ": " << parents[i] << "\n";
        }
        // wypisuje tablic� przep�yw�w
        cout << "\nflows:\n";
        for (int i = 0; i < flows.size(); i++)
        {
            cout << i << ": " << flows[i] << "\n";
        }

        int finish = vertices - 1;
        /// nie znaleziono sciezki do uj�cia, koniec algorytmu FF
        if (parents[finish] == -1) break;

        // zapisz �cie�k� powi�kszaj�c� na podstawie tablicy poprzednik�w
        vector<int> shortestPath; // tutaj b�dzie zapisywana �cie�ka
        cout << "\nShortest path:\n";
        
        while (finish != start)
        {
            shortestPath.push_back(finish);
            finish = parents[finish];
            
        }
        shortestPath.push_back(finish); // dodaj jeszcze pocz�tkowy wierzcho�ek
        finish = vertices - 1;
        reverse(shortestPath.begin(), shortestPath.end());

        // wymie� wierzcho�ki tworz�ce �cie�k� powi�kszaj�c�
        for (int i = 0; i < shortestPath.size(); i++)
        {
            cout << shortestPath[i] << ", ";
        }
        cout << endl;

        cout << "Max Flow In This Path: " << flows[finish] << endl;
        totalFlow += flows[finish];

        // zaktualizuj przep�yw w li�cie s�siedztwa
        int v;
        for (int i = 0; i < shortestPath.size() - 1; i++) // dla ka�dego w�z�a w �cie�ce powi�kszaj�cej (bez ostatniego)
        {
            v = shortestPath[i];
            for (int j = 0; j < aList[v].size(); j++) // przeszukaj jego list� s�siedztwa
            {
                if (aList[v][j].to == shortestPath[i + 1]) // �eby znale�� ten, kt�ry jest nast�pny w �cie�ce powi�kszaj�cej
                {
                    aList[v][j].remainingFlow -= flows[finish]; // odejmij przep�yw
                    // zaktualizuj przep�yw w przeciwnym kierunku
                    for (j = 0; j < aList[shortestPath[i + 1]].size(); j++) // znajd� strza�k� w drug� stron�
                    {
                        if (aList[shortestPath[i + 1]][j].to == shortestPath[i])
                        {
                            aList[shortestPath[i + 1]][j].remainingFlow += flows[finish]; // dodaj przep�yw w przeciwnym kierunku
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
    // 1a. Przerobi� ten plik tak, �eby wczytywa� z pliku node'y zamiast po��cze�




    ifstream plik("daneZwagami.txt");
    //zmienne do wczytania pliku z kraw�dziami
    int vertices, newEdge1, newEdge2, maxFlow;
    //string line;
    // wczyta pierwszy wiersz (liczba w�z��w i kraw�dzi)
    // wczytywanie kraw�dzi jest niepotrzebne, �eby �atwiej by�o przekopiowa� do visualgo
    if (!(plik >> vertices >> newEdge1))
    {
        return -1;
    }
    // tworzy list� s�siedztwa
    vector<vector<Road>> aList(vertices);

    while (plik >> newEdge1 >> newEdge2 >> maxFlow)
    {
        // tworzy tymczasow� struktur�, uzupe�nia danymi z pliku i dodaje do listy s�siedztwa
        Road roadTemp;
        roadTemp.to = newEdge2;
        roadTemp.remainingFlow = maxFlow;
        aList[newEdge1].push_back(roadTemp);


        // dodaje do listy s�siedztwa po��czenie w przeciwnym kierunku z zerowym przep�ywem
        // TODO: do this only when path doesn't already exist
        roadTemp.to = newEdge1;
        roadTemp.remainingFlow = 0;
        aList[newEdge2].push_back(roadTemp);
    }

    cout << "Max Flow In Whole Path: " << fordFulkerson(aList) << endl;

    return 0;
}