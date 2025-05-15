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
    Q.push(start); // na pocz�tek dodaje w�ze� startowy do kolejki
    int v = 0;
    while (!Q.empty())
    {
        int u = Q.front(); // zapisuje element na pocz�tku kolejki
        Q.pop(); // usuwa element z pocz�tku kolejki
        for (size_t i = 0; i < this->nList[u].size(); i++) // dla ka�dej drogi z u
        {
            v = this->nList[u][i].v;
            // dodaje w�ze� v do kolejki je�li nieodwiedzony i ma dost�pny przep�yw
            if (visited[v] == false && this->nList[u][i].remainingFlow > 0)
            {   // po dodaniu:
                visited[v] = true; // w�ze� ma status odwiedzony
                parents[v] = u; // zapisuje u jako poprzednik v
                flows[v] = min(flows[u], this->nList[u][i].remainingFlow); // aktualizuje maksymalny przep�yw dla w�z�a
                Q.push(v); // dodaje w�ze� v do kolejki
            }
        }
    }
}

double AdjacencyList::edmonsKarp()
{
    double totalFlow = 0;
    while (true) // wykonuje, a� nie znajdzie �cie�ki powi�kszaj�cej
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
        int start = 0; // indeks w�z�a startowego
        vector<int> parents(this->vertices, -1); // poprzedniki, na pocz�tku wszystkie -1
        vector<double> flows(this->vertices, INT_MAX); // maksymalny przep�yw do ka�dego w�z�a

        bfsFlow(this->nList, parents, flows, 0); // BFS

        // wypisuje tablic� poprzednik�w
        cout << "\nparents:\n";
        for (size_t i = 0; i < parents.size(); i++)
        {
            cout << i << ": " << parents[i] << "\n";
        }
        // wypisuje tablic� przep�yw�w
        cout << "\nflows:\n";
        for (size_t i = 0; i < flows.size(); i++)
        {
            cout << i << ": " << flows[i] << "\n";
        }

        int finish = this->vertices - 1;
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
        finish = this->vertices - 1;
        reverse(shortestPath.begin(), shortestPath.end());

        // wymie� wierzcho�ki tworz�ce �cie�k� powi�kszaj�c�
        for (size_t i = 0; i < shortestPath.size(); i++)
        {
            cout << shortestPath[i] << ", ";
        }
        cout << endl;

        cout << "Max Flow In This Path: " << flows[finish] << endl;
        totalFlow += flows[finish];

        // zaktualizuj przep�yw w li�cie s�siedztwa
        int v;
        bool foundOppositeDirection; // do sprawdzania, czy znaleziono drog� w przeciwnym kierunku
        for (size_t i = 0; i < shortestPath.size() - 1; i++) // dla ka�dego w�z�a w �cie�ce powi�kszaj�cej (bez ostatniego)
        {
            foundOppositeDirection = false;
            v = shortestPath[i];
            for (size_t j = 0; j < this->nList[v].size(); j++) // przeszukaj jego list� s�siedztwa
            {
                if (this->nList[v][j].v == shortestPath[i + 1]) // �eby znale�� ten, kt�ry jest nast�pny w �cie�ce powi�kszaj�cej
                {
                    this->nList[v][j].remainingFlow -= flows[finish]; // odejmij przep�yw
                    // zaktualizuj przep�yw w przeciwnym kierunku
                    for (size_t j = 0; j < this->nList[shortestPath[i + 1]].size(); j++) // znajd� strza�k� w drug� stron�
                    {
                        if (this->nList[shortestPath[i + 1]][j].v == shortestPath[i])
                        {
                            this->nList[shortestPath[i + 1]][j].remainingFlow += flows[finish]; // dodaj przep�yw w przeciwnym kierunku
                            foundOppositeDirection = true; // droga w przeciwnym kierunku b->a znaleziona
                            break; // droga b->a znaleziona, nie szukaj dalej
                        }
                    }
                    break; // droga a->b znaleziona, nie szukaj dalej
                }
            }
            if (!foundOppositeDirection)
            {   // nie znaleziono drogi b->a
                addEdge(shortestPath[i + 1], shortestPath[i], 0); // utw�rz nowe po��czenie
                this->nList[shortestPath[i + 1]][this->nList[shortestPath[i + 1]].size() - 1].remainingFlow += flows[finish]; // dodaj przep�yw w nowym, przeciwnym kierunku
            }
        }
    }
    return totalFlow;
}


