#include "AdjacencyList.h"

AdjacencyList::AdjacencyList() {
    vertices = 0;
    nList.clear();
}

AdjacencyList::AdjacencyList(int n) {
    vertices = n;
    nList.resize(n);
}

void AdjacencyList::addEdge(int u, int v, double value, double cost) {
	nList[u].push_back(EdgeData(v, value, cost));
}

bool AdjacencyList::readFileToGraph(string fileName) {
    ifstream plik(fileName);
    int vertices;
    int newEdge1, newEdge2, maxFlow, cost;
    string line;

    if (!(plik >> vertices >> newEdge1))
    {
        cerr << "Nie mo?na otworzy? pliku ";
        return false;
    }

        this->vertices = vertices;
        this->nList.resize(vertices);
        this->source = 0;
        this->target = vertices - 1;

        while (plik >> newEdge1 >> newEdge2 >> maxFlow >> cost)
        {
            addEdge(newEdge1, newEdge2, maxFlow, cost);
        }
        plik.close();
        return true;
}

void AdjacencyList::printGraph() {
	for (int i = 0; i < vertices; i++) {
		cout << i << ": ";
		for (EdgeData edge : nList[i]) {
			cout << "(" << edge.v << " : " << edge.remainingFlow << "[" << edge.cost << "] )";
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
        //cout << "\aLista sasiedztwa: z: ( do, pozostaly ):\n";
        //for (size_t i = 0; i < this->vertices - 1; i++)
        //{
        //    cout << i << ": \t";
        //    for (size_t j = 0; j < this->nList[i].size(); j++)
        //    {
        //        cout << "( " << this->nList[i][j].v << ", " << this->nList[i][j].remainingFlow << " )\t";
        //    }
        //    cout << endl;
        //}
        // BFS
        int start = 0; // indeks wêz³a startowego
        vector<int> parents(this->vertices, -1); // poprzedniki, na pocz¹tku wszystkie -1
        vector<double> flows(this->vertices, INT_MAX); // maksymalny przep³yw do ka¿dego wêz³a

        bfsFlow(this->nList, parents, flows, 0); // BFS

        // wypisuje tablicê poprzedników
        //cout << "\nparents:\n";
        //for (size_t i = 0; i < parents.size(); i++)
        //{
        //    cout << i << ": " << parents[i] << "\n";
        //}
        //// wypisuje tablicê przep³ywów
        //cout << "\nflows:\n";
        //for (size_t i = 0; i < flows.size(); i++)
        //{
        //    cout << i << ": " << flows[i] << "\n";
        //}

        int finish = this->vertices - 1;
        /// nie znaleziono sciezki do ujœcia, koniec algorytmu FF
        if (parents[finish] == -1) break;

        // zapisz œcie¿kê powiêkszaj¹c¹ na podstawie tablicy poprzedników
        vector<int> shortestPath; // tutaj bêdzie zapisywana œcie¿ka
 /*       cout << "\nShortest path:\n";*/

        while (finish != start)
        {
            shortestPath.push_back(finish);
            finish = parents[finish];

        }
        shortestPath.push_back(finish); // dodaj jeszcze pocz¹tkowy wierzcho³ek
        finish = this->vertices - 1;
        reverse(shortestPath.begin(), shortestPath.end());

        // wymieñ wierzcho³ki tworz¹ce œcie¿kê powiêkszaj¹c¹
 /*       for (size_t i = 0; i < shortestPath.size(); i++)
        {
            cout << shortestPath[i] << ", ";
        }
        cout << endl;

        cout << "Max Flow In This Path: " << flows[finish] << endl;*/
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
                addEdge(shortestPath[i + 1], shortestPath[i], 0,0); // utwórz nowe po³¹czenie
                this->nList[shortestPath[i + 1]][this->nList[shortestPath[i + 1]].size() - 1].remainingFlow += flows[finish]; // dodaj przep³yw w nowym, przeciwnym kierunku
            }
        }
    }
    return totalFlow;
}

void AdjacencyList::createResidualNetwork(AdjacencyList& temp) {
    this->nList.clear();
    this->nList.resize(temp.vertices + 1);
    this->vertices = temp.vertices + 1;

    for (int u = 0; u < temp.vertices; ++u) {
        for (const EdgeData& edge : temp.nList[u]) {
            int v = edge.v;
            double flow = edge.flow;
            double cost = edge.cost;

            // Kraw?d? normalna (u -> v)
            this->nList[u].push_back(EdgeData(v, flow, cost));

            // Kraw?d? odwrotna (v -> u) z przepustowo?ci? 0 i przeciwnym kosztem
            // Dodaj tylko je?li jeszcze nie istnieje
            bool found = false;
            for (const EdgeData& back : this->nList[v]) {
                if (back.v == u) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                this->nList[v].push_back(EdgeData(u, 0, -cost));
            }
        }
    }
}


//algorytm znajdowania najkrotszych sciezek. Zmodyfikowany pod nasz problem
bool AdjacencyList::dijkstraModify(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf) {
    const double INF = numeric_limits<double>::max();
    vector<double> d(vertices + 1, INF);
    vector<int> visited(vertices + 1, 0);
    d[source] = 0;

    // Kolejka priorytetowa - pary {odleg?o??, wierzcho?ek}
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    pq.push({ 0, source });

    while (!pq.empty()) {
        double dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = 1;

        if (u == target) break;// znaleziono najkrótsz? ?cie?k? do t

        // Teraz iterujemy po li?cie s?siadów (lista s?siedztwa)
        for (const EdgeData& edge : graf[u]) {
            int v = edge.v;
            if (edge.remainingFlow != 0) {
                double weight = edge.cost;
                if (d[u] + weight < d[v]) {
                    d[v] = d[u] + weight;
                    parents[v] = u;
                    pq.push({ d[v], v });
                }
            }
        }
    }

    // Wy?wietlenie wyników
    //cout << "Odleg?o?ci od wierzcho?ka " << source << ":\n";

    if (d[target] == INF) {
        return false;
    }
    else {
        /*for (int i = 0; i < vertices; i++) {
            cout << i << " : " << d[i] << endl;
        }*/
        cost = d[target];
        return true;
    }
}

//algorytm znajdowania najkrotszych sciezek.
bool AdjacencyList::BellmanFord(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf) {
    const double INF = numeric_limits<double>::max();
    vector<double> d(vertices + 1, INF); // dystanse
    d[source] = 0;

    for (int k = 0; k < vertices; ++k) {
        for (int i = 0; i < vertices; ++i) {
            for (const EdgeData& edge : graf[i]) {
                int v = edge.v;
                if (edge.remainingFlow != 0 && d[v] > d[i] + edge.cost) {
                    d[v] = d[i] + edge.cost;
                    parents[v] = i;
                }
            }
        }
    }
    // Wyniki
    if (d[target] == INF) {
        return false;
    }
    else {
        /*for (int i = 0; i < vertices; i++) {
            cout << i << " : " << d[i] << endl;
        }*/
        cost = d[target];
        return true;
    }
};

double AdjacencyList::BusackerGowen2(double const maxFlow, int s, int t, vector<Path>& roads,
    bool (AdjacencyList::* shortestPathFunc)(int, int, double&, vector<int>&, const vector<vector<EdgeData>>&)) {

    double result = 0; //przeplyw, zwieksza si? a? nie osiagnie warto?ci F- tj. maxFlow. Podanego jako argument funkcji.
    double summaricResult = 0; //Suma (cost * przeplyw)-jedna sciezka
    double sumCost = 0; // Suma kosztow sciezek
    vector<int> f(vertices); //tablica ojcow
    double cost; //koszt jednej sciezki
    vector<int> path; // lista sciezek do wypisanie

    AdjacencyList siecResidualna(vertices);

    siecResidualna.createResidualNetwork(*this);


    // Wykonuje dopoki nie zostal osiagniety przeplyw F(maxFlow)
    // i dopoki istnieje sciezka z s do t (wybiera t? sci??k? która ma najni?szy koszt)
    while (result < maxFlow && (this->*shortestPathFunc)(s, t, cost, f, siecResidualna.nList)) {

        int tmpT = t;
        int tmp = f[t];

        // znajd? pierwsz? kraw?d? (tmp -> tmpT)
        EdgeData* e = EdgeData::findEdge(siecResidualna.nList[tmp], tmpT);
        double maxFlowOfPath = e ? e->remainingFlow : 0;

        while (tmp != s) {
            tmpT = tmp;
            tmp = f[tmp];
            EdgeData* edge = EdgeData::findEdge(siecResidualna.nList[tmp], tmpT);
            if (edge) {
                maxFlowOfPath = min(maxFlowOfPath, edge->remainingFlow);
            }
        }

        if (result + maxFlowOfPath > maxFlow) {
            maxFlowOfPath = maxFlow - result;
        }

        // aktualizacja sieci residualnej
        tmpT = t;
        tmp = f[t];
        while (tmp != s) {
            EdgeData* forward = EdgeData::findEdge(siecResidualna.nList[tmp], tmpT);
            EdgeData* rev = EdgeData::findEdge(siecResidualna.nList[tmpT], tmp);

            if (forward) forward->remainingFlow -= maxFlowOfPath;
            if (rev) {
                rev->remainingFlow += maxFlowOfPath;
                rev->cost = -(forward ? forward->cost : rev->cost); // odwró? koszt
            }

            path.push_back(tmpT);
            tmpT = tmp;
            tmp = f[tmp];
        }

        // ostatni krok
        EdgeData* forward = EdgeData::findEdge(siecResidualna.nList[tmp], tmpT);
        EdgeData* rev = EdgeData::findEdge(siecResidualna.nList[tmpT], tmp);
        if (forward) forward->remainingFlow -= maxFlowOfPath;
        if (rev) {
            rev->remainingFlow += maxFlowOfPath;
            rev->cost = -(forward ? forward->cost : rev->cost);
        }
        path.push_back(tmpT);
        path.push_back(s);
        reverse(path.begin(), path.end());

        roads.push_back(Path(path, maxFlowOfPath, cost));
        result += maxFlowOfPath;
        summaricResult += (maxFlowOfPath * cost);

        //Wypisanie sciezek
        cout << "Sciezka: ";

        for (int val : path) {
            cout << val << " ";
        }

        cout << " Mozna przeslac : " << maxFlowOfPath << "j.kosztem : " << cost << ". Zatem koszt sciezki: " << (maxFlowOfPath * cost) << endl;
        path.clear();
        sumCost += cost;
    }

    //wypisanie rezultatow
    cout << "Sumaryczny koszt: " << summaricResult << endl;
    cout << "Suma kosztów dróg: " << sumCost << endl;

    //wyjatek
    if (result != maxFlow) {
        cout << "UWAGA! Nie osi?gni?to maksymalnego przeplywu. GDZIES JEST BLAD";
    }

    return summaricResult;
}


// Klasyczny minCostMaxFlow. Z wykorzystaniem algorytmu BusackeraGowena
void AdjacencyList::maxFlowMinCost() {
    vector<Path> a;
    double maxFlow = edmonsKarp();

    cout <<"max flow" << maxFlow << endl;
    BusackerGowen2(maxFlow, source, target, a, &AdjacencyList::dijkstraModify);
    cout << endl;
    BusackerGowen2(maxFlow, source, target , a, &AdjacencyList::BellmanFord);
}



