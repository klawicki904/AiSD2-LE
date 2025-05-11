#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() {
    vertices = 0;
    nList.clear();
}

AdjacencyList::AdjacencyList(int n) {
    vertices = n;
    nList.resize(n);
}


void AdjacencyList::addEdge(int u, int v, double value) {
	nList[u].push_back(EdgeData(v, value));
}

bool AdjacencyList::readFileToGraph(string fileName) {
    ifstream plik(fileName);
    int vertices;
    int newEdge1, newEdge2, maxFlow;
    string line;

    if (!(plik >> vertices >> newEdge1))
    {
        cerr << "Nie mo?na otworzy? pliku ";
        return false;
    }

        this->vertices = vertices;
        this->nList.resize(vertices);

        while (plik >> newEdge1 >> newEdge2 >> maxFlow)
        {
            addEdge(newEdge1, newEdge2, maxFlow);
        }
        plik.close();
        return true;
}

void AdjacencyList::printGraph() {
	for (int i = 0; i < vertices; i++) {
		cout << i << ": ";
		for (EdgeData edge : nList[i]) {
			cout << "(" << edge.v << " : " << edge.remainingFlow << " )";
		}
		cout << endl;
	}
}

void AdjacencyList::bfsFlow(vector<vector<EdgeData>>& aList, vector<int>& parents, vector<double>& flows, int start )
{
    vector<bool> visited(this->vertices, false); // odwiedzone 
    queue<int> Q; // kolejka
    //parents[start] = -2;

    visited[start] = 1;
    Q.push(start); // na pocz¹tek dodaje wêze³ startowy do kolejki
    int v = 0;
    while (!Q.empty())
    {
        int u = Q.front(); // zapisuje element na pocz¹tku kolejki
        Q.pop(); // usuwa element z pocz¹tku kolejki
        for (size_t i = 0; i < this->nList[u].size(); i++) // dla ka¿dej drogi z u
        {
            v = this->nList[u][i].v;
            // dodaje wêze³ v do kolejki jeœli nieodwiedzony i ma dostêpny przep³yw
            if (visited[v] == false && this->nList[u][i].remainingFlow > 0)
            {   // po dodaniu:
                visited[v] = true; // wêze³ ma status odwiedzony
                parents[v] = u; // zapisuje u jako poprzednik v
                flows[v] = min(flows[u], this->nList[u][i].remainingFlow); // aktualizuje maksymalny przep³yw dla wêz³a
                Q.push(v); // dodaje wêze³ v do kolejki
            }
        }
    }
}

double AdjacencyList::edmonsKarp()
{
    double totalFlow = 0;
    while (true) // wykonuje, a¿ nie znajdzie œcie¿ki powiêkszaj¹cej
    {
        /// wyswietla stan listy sasiedztwa
        cout << "\aLista sasiedztwa: z: ( do, pozostaly ):\n";
        for (size_t i = 0; i < this->vertices - 1; i++)
        {
            cout << i << ": \t";
            for (size_t j = 0; j < this->nList[i].size(); j++)
            {
                cout << "( " << this->nList[i][j].v << ", " << this->nList[i][j].remainingFlow << " )\t";
            }
            cout << endl;
        }
        // BFS
        int start = 0; // indeks wêz³a startowego
        vector<int> parents(this->vertices, -1); // poprzedniki, na pocz¹tku wszystkie -1
        vector<double> flows(this->vertices, INT_MAX); // maksymalny przep³yw do ka¿dego wêz³a

        bfsFlow(this->nList, parents, flows, 0); // BFS

        // wypisuje tablicê poprzedników
        cout << "\nparents:\n";
        for (size_t i = 0; i < parents.size(); i++)
        {
            cout << i << ": " << parents[i] << "\n";
        }
        // wypisuje tablicê przep³ywów
        cout << "\nflows:\n";
        for (size_t i = 0; i < flows.size(); i++)
        {
            cout << i << ": " << flows[i] << "\n";
        }

        int finish = this->vertices - 1;
        /// nie znaleziono sciezki do ujœcia, koniec algorytmu FF
        if (parents[finish] == -1) break;

        // zapisz œcie¿kê powiêkszaj¹c¹ na podstawie tablicy poprzedników
        vector<int> shortestPath; // tutaj bêdzie zapisywana œcie¿ka
        cout << "\nShortest path:\n";

        while (finish != start)
        {
            shortestPath.push_back(finish);
            finish = parents[finish];

        }
        shortestPath.push_back(finish); // dodaj jeszcze pocz¹tkowy wierzcho³ek
        finish = this->vertices - 1;
        reverse(shortestPath.begin(), shortestPath.end());

        // wymieñ wierzcho³ki tworz¹ce œcie¿kê powiêkszaj¹c¹
        for (size_t i = 0; i < shortestPath.size(); i++)
        {
            cout << shortestPath[i] << ", ";
        }
        cout << endl;

        cout << "Max Flow In This Path: " << flows[finish] << endl;
        totalFlow += flows[finish];

        // zaktualizuj przep³yw w liœcie s¹siedztwa
        int v;
        bool foundOppositeDirection; // do sprawdzania, czy znaleziono drogê w przeciwnym kierunku
        for (size_t i = 0; i < shortestPath.size() - 1; i++) // dla ka¿dego wêz³a w œcie¿ce powiêkszaj¹cej (bez ostatniego)
        {
            foundOppositeDirection = false;
            v = shortestPath[i];
            for (size_t j = 0; j < this->nList[v].size(); j++) // przeszukaj jego listê s¹siedztwa
            {
                if (this->nList[v][j].v == shortestPath[i + 1]) // ¿eby znaleŸæ ten, który jest nastêpny w œcie¿ce powiêkszaj¹cej
                {
                    this->nList[v][j].remainingFlow -= flows[finish]; // odejmij przep³yw
                    // zaktualizuj przep³yw w przeciwnym kierunku
                    for (size_t j = 0; j < this->nList[shortestPath[i + 1]].size(); j++) // znajdŸ strza³kê w drug¹ stronê
                    {
                        if (this->nList[shortestPath[i + 1]][j].v == shortestPath[i])
                        {
                            this->nList[shortestPath[i + 1]][j].remainingFlow += flows[finish]; // dodaj przep³yw w przeciwnym kierunku
                            foundOppositeDirection = true; // droga w przeciwnym kierunku b->a znaleziona
                            break; // droga b->a znaleziona, nie szukaj dalej
                        }
                    }
                    break; // droga a->b znaleziona, nie szukaj dalej
                }
            }
            if (!foundOppositeDirection)
            {   // nie znaleziono drogi b->a
                addEdge(shortestPath[i + 1], shortestPath[i], 0); // utwórz nowe po³¹czenie
                this->nList[shortestPath[i + 1]][this->nList[shortestPath[i + 1]].size() - 1].remainingFlow += flows[finish]; // dodaj przep³yw w nowym, przeciwnym kierunku
            }
        }
    }
    return totalFlow;
}


