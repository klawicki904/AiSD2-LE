
#include "gtest/gtest.h"
#include "Matrix.h"
#include <string>
#include <sstream>
#include <functional>



// Konstruktor
Matrix::Matrix() : vertices(0) {
}

Matrix::Matrix(int n) : vertices(n) {
    tab.resize(n, vector<EdgeData>(n));  // Inicjalizuje macierz o rozmiarze n x n, wype?nion? zerami
}

void Matrix::init(int n) {
    vertices = n;
    tab.resize(n, vector<EdgeData>(n));
}

// Funkcja do dodawania kraw?dzi (u, v) z wag? 'weight'
void Matrix::addEdge(int u, int v, double weight, double cost) {
    tab[u][v] = EdgeData(v, weight, cost);  // Dodaje wag? kraw?dzi w macierzy
}

void Matrix::addNode(const Node& node) {
    int id = node.GetId();

    if (id >= listVertives.size()) {
        listVertives.resize(id + 1); // powi?kszamy tablic? je?li trzeba
    }

    listVertives[id] = node;
}

// Medota wypisuje graf
void Matrix::printGraph() const {
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            cout << tab[i][j].remainingFlow << "[" <<  tab[i][j].cost <<"]" << " ";
        }
        cout << endl;
    }
}

bool Matrix::readFileToGraph(string fileName) {
    ifstream plik(fileName);
    int vertices; // ilosc wierzcholkow
    int u, v, edges;// u , v (u --> v), przepustowosc, ilosc krawedzi 
    double maxFlow, x, y, capacity, cost = 0.0;
    string line;
    int nodeId;

    if (!(plik >> vertices >> edges))
    {
        cerr << "Nie mo?na otworzy? pliku ";
        return false;
    }
    //inicjalizuje macierz
    this->init(vertices);

    // Wczytywanie kraw?dzi
    for (int i = 0; i < edges; i++) {
        getline(plik, line);
        if (line.empty()) {
            i--; // ignoruj puste linie
            continue;
        }

        istringstream iss(line);

        if (!(iss >> u >> v >> maxFlow)) {
            std::cerr << "B??d podczas wczytywania kraw?dzi w linii: " << line << std::endl;
            continue;
        }

        if (!(iss >> cost)) {
            cost = 0.0; // Je?li brak kosztu, przypisz domy?lnie
        }

        addEdge(u, v, maxFlow, cost);
    }


    // Wczytywanie Node'?w
    while (plik >> nodeId >> x >> y >> line >> capacity) {
        Node::NodeType type = Node::NodeType::None;

        if (line == "Field") type = Node::NodeType::Field;
        else if (line == "Brewery") type = Node::NodeType::Brewery;
        else if (line == "Pub") type = Node::NodeType::Pub;

        Node tmpNode(nodeId, x, y, capacity, type);
        addNode(tmpNode);
    }

    plik.close();
    return true;
}

void Matrix::dijkstra(int source) {
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

        for (int v = 0; v < vertices; ++v) {
            if (tab[u][v].remainingFlow != 0) {
                double weight = tab[u][v].cost;
                if (d[u] + weight < d[v]) {
                    d[v] = d[u] + weight;
                    pq.push({ d[v], v });
                }
            }
        }
    }

    // Wy?wietlenie wyników
   /* cout << "Odleg?o?ci od wierzcho?ka " << source << ":\n";
    for (int i = 0; i < vertices; ++i) {
        if (d[i] == INF) {
            cout << "Do wierzcho?ka " << i << " nie ma ?cie?ki.\n";
        }
        else {
            cout << "Odleg?o?? do wierzcho?ka " << i << ": " << d[i] << "\n";
        }
    }*/
}

bool Matrix::BellmanFord(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf) {
    const double INF = numeric_limits<double>::max();
    vector<double> d(vertices + 1, INF); // dystanse
    d[source] = 0;

    for (int k = 0; k < vertices; ++k) {
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                if (graf[i][j].remainingFlow != 0 && d[i] != numeric_limits<double>::max()) {
                    if (d[j] > d[i] + graf[i][j].cost) {
                        d[j] = d[i] + graf[i][j].cost;
                        parents[j] = i;
                    }
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

bool Matrix::dijkstraModify(int source, int target, double& cost, vector<int>& parents, const vector<vector<EdgeData>>& graf) {
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

        for (int v = 0; v < vertices; ++v) {
            if (graf[u][v].remainingFlow != 0) {
                double weight = graf[u][v].cost;
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

//Algorytm Busackera_Gowena wyznacza najta?szy przep?yw w sieci dla docelowego przep?ywu F.
double Matrix::BusackerGowen(double const maxFlow, 
    bool (Matrix::*shortestPathFunc)(int, int, double&, vector<int>&, const vector<vector<EdgeData>>&)) {
    int s = 0;
    int t = vertices - 1;
    double result = 0; //przeplyw, zwieksza si? a? nie osiagnie warto?ci F- tj. maxFlow. Podanego jako argument funkcji.
    double summaricResult = 0; //Suma (cost * przeplyw)-jedna sciezka
    double sumCost = 0; // Suma kosztow sciezek

    // UWAGA !! Mozna przerobic aby dzialalo na orginale. Bez pomocniczej sieci, ale wtedy na sta?e graf zostaje zmodyfikowany.
    Matrix siecResidualna(vertices); //siec residualna

    vector<int> f(vertices); //tablica ojcow
    double cost; //koszt jednej sciezki

    //Siec residualna na start algorytmu jest taka sama jak siec przep?ywowa
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            siecResidualna.tab[i][j] = tab[i][j];
        }
    }
        // Wykonuje dopoki nie zostal osiagniety przeplyw F(maxFlow)
        // i dopoki istnieje sciezka z s do t (wybiera t? sci??k? która ma najni?szy koszt)
        while (result < maxFlow && (this->*shortestPathFunc)(s, t, cost, f, siecResidualna.tab)) {
            //siecResidualna.printGraph();
            //cout << "Znalazlem sciezke o koszcie: " << cost << endl;
            /*for (int i = 0; i < vertices; i++) {
                cout << i << " : " << f[i] << endl;
            }*/
            int tmpT = t;
            int tmp = f[t];
            // cout << " Droga: z " << s << " do " << t << endl;


            double maxFlowOfPath = siecResidualna.tab[tmp][tmpT].remainingFlow;

            //Wypisuje siezki powieszajace i wyznacza minimum przepustowosci z tej sciezki
            while (tmp != s) {
                // cout << tmpT << " -> " << tmp << " min: " << maxFlowOfPath << endl;
                tmpT = tmp;
                tmp = f[tmp];
                if (maxFlowOfPath > siecResidualna.tab[tmp][tmpT].remainingFlow) {
                    maxFlowOfPath = siecResidualna.tab[tmp][tmpT].remainingFlow;
                }
            }
            //cout << "Minimum" << maxFlowOfPath << endl;



            if (result + maxFlowOfPath > maxFlow) {
                maxFlowOfPath = maxFlow - result;
            }

            tmpT = t;
            tmp = f[t];
            //aktualizuje siec residualna
            while (tmp != s) {
                siecResidualna.tab[tmp][tmpT].remainingFlow -= maxFlowOfPath;
                siecResidualna.tab[tmpT][tmp].remainingFlow += maxFlowOfPath;
                siecResidualna.tab[tmpT][tmp].cost = -siecResidualna.tab[tmp][tmpT].cost;
                tmpT = tmp;
                tmp = f[tmp];
            }
            siecResidualna.tab[tmp][tmpT].remainingFlow -= maxFlowOfPath;
            siecResidualna.tab[tmpT][tmp].remainingFlow += maxFlowOfPath;
            siecResidualna.tab[tmpT][tmp].cost = -siecResidualna.tab[tmp][tmpT].cost;

            result += maxFlowOfPath;
            //cout << "Wynik: " << result << endl;
            summaricResult += (maxFlowOfPath * cost);
            cout << "Sciezka mozna przeslac: " << maxFlowOfPath << "j. kosztem: " << cost << endl;
            cout << "Zatem koszt sciezki: " << (maxFlowOfPath * cost) << endl;
            sumCost += cost;
        }
        cout << "Sumaryczny koszt: " << summaricResult << endl;
        cout << "Suma kosztów dróg: " << sumCost << endl;

        return summaricResult;
}

void Matrix::maxFlowMinCost() {
    double maxFlow = edmonsKarp();
    BusackerGowen(maxFlow, &Matrix::dijkstraModify);
    cout << endl;
    BusackerGowen(maxFlow, &Matrix::BellmanFord);
}



// Klasyczny fordFulkersow
double Matrix::edmonsKarp() {
    int s = 0;
    int t = vertices - 1;

    double maxFlow = 0; //maksymalny przeplyw
    Matrix siecResidualna(vertices); //siec residualna

    //Siec residualna na start algorytmu jest taka sama jak siec przep?ywowa
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            siecResidualna.tab[i][j] = tab[i][j];

        }
    }
    //siecResidualna.printMatrix();
    vector<int> f(vertices); //tablica ojcow

    while (bfs(s, siecResidualna.tab, t, f))
    {
        int tmpT = t;
        int tmp = f[t];
        // cout << " Droga: z " << s << " do " << t << endl;
        double min = siecResidualna.tab[tmp][tmpT].remainingFlow;

        //Wypisuje siezki powieszajace i wyznacza minimum przepustowosci z tej sciezki
        while (tmp != s) {
            //cout << tmpT << " -> " << tmp << " min: " << min << endl;
            tmpT = tmp;
            tmp = f[tmp];
            if (min > siecResidualna.tab[tmp][tmpT].remainingFlow) {
                min = siecResidualna.tab[tmp][tmpT].remainingFlow;
            }
        }
        //cout << tmpT << " -> " << tmp << " min: " << min << endl;

        tmpT = t;
        tmp = f[t];
        //aktualizuje siec residualna
        while (tmp != s) {
            siecResidualna.tab[tmp][tmpT].remainingFlow -= min;
            siecResidualna.tab[tmpT][tmp].remainingFlow += min;
            tmpT = tmp;
            tmp = f[tmp];
        }
        siecResidualna.tab[tmp][tmpT].remainingFlow -= min;
        siecResidualna.tab[tmpT][tmp].remainingFlow += min;

        //dodaje minimum z tej ?cie?ki do wyniku, jako maksymalny przep?yw
        maxFlow += min;
    }
    return maxFlow;
}

//Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
bool Matrix::bfs(int s, const vector<vector<EdgeData>>& graf, int t, vector<int>& f) {
    vector<int> visited(vertices, 0);
    queue<int> kolejka;

    visited[s] = 1;
    kolejka.push(s);

    while (!kolejka.empty()) {
        int x = kolejka.front();
        kolejka.pop();

        visited[x] = 1;
        for (int u = 1; u < vertices; u++) {
            if (graf[x][u].remainingFlow > 0 && visited[u] == 0) {
                visited[u] = 1;
                kolejka.push(u);
                f[u] = x;

                if (u == t) {
                    return true;
                }
            }

        }
    }
    return false;
}

///Ford Fulkerson dla sieci w ktorej interesuja nas sciezki powiekszajace s-> ... -> t 
// (Gdzie gdzies po drodze jest wierzcholek ktory jest browarem)
double Matrix::edmonsKarp2() {
    int s = 0;
    int t = vertices - 1;
    int midT = vertices;
    double maxFlow = 0, maxFlow1 = 0; //maksymalny przeplyw
    Matrix siecResidualna(vertices + 1); //siec residualna

    //Siec residualna na start algorytmu jest taka sama jak siec przep?ywowa
    for (int i = 0; i <= vertices; i++) {
        for (int j = 0; j <= vertices; j++) {
            if ((j < vertices) && (i < vertices)) {
                siecResidualna.tab[i][j] = tab[i][j];
            }
            else {
                siecResidualna.tab[i][j] = EdgeData();
            }
        }
    }

    for (Node i : listVertives) {

        if (i.GetType() == Node::NodeType::Brewery) {
            siecResidualna.tab[i.GetId()][midT].remainingFlow = numeric_limits<double>::max();
            for (int j = 1; j < vertices; j++) {
                if (tab[i.GetId()][j].remainingFlow > 0) {
                    siecResidualna.tab[midT][j].remainingFlow = numeric_limits<double>::max();
                }
            }
        }
    }
    // siecResidualna.printGraph();
    vector<int> f(vertices + 1); //tablica ojcow
    vector<int> f2(vertices + 1); //tablica ojcow

    s = 0;
    t = midT;
    while (bfs2(s, siecResidualna.tab, midT, f))
    {
        int tmpT = t;
        int tmp = f[t];
         cout << " Droga: z " << s << " do " << t << endl;
        double min = siecResidualna.tab[tmp][tmpT].remainingFlow;

        //Wypisuje siezki powieszajace i wyznacza minimum przepustowosci z tej sciezki
        while (tmp != s) {
            cout << tmpT << " -> " << tmp << " min: " << min << endl;
            tmpT = tmp;
            tmp = f[tmp];
            if (min > siecResidualna.tab[tmp][tmpT].remainingFlow) {
                min = siecResidualna.tab[tmp][tmpT].remainingFlow;
            }
        }
        cout << tmpT << " -> " << tmp << " min: " << min << endl;

        tmpT = t;
        tmp = f[t];
        //aktualizuje siec residualna
        while (tmp != s) {
            siecResidualna.tab[tmp][tmpT].remainingFlow -= min;
            siecResidualna.tab[tmpT][tmp].remainingFlow += min;
            tmpT = tmp;
            tmp = f[tmp];
        }
        siecResidualna.tab[tmp][tmpT].remainingFlow -= min;
        siecResidualna.tab[tmpT][tmp].remainingFlow += min;
        //dodaje minimum z tej ?cie?ki do wyniku, jako maksymalny przep?yw
        maxFlow += min;
    }

    s = midT;
    t = vertices - 1;
    while (bfs2(midT, siecResidualna.tab, t, f2))
    {
        int tmpT = t;
        int tmp = f2[t];
         cout << " Droga: z " << s << " do " << t << endl;
        double min = siecResidualna.tab[tmp][tmpT].remainingFlow;

        //Wypisuje siezki powieszajace i wyznacza minimum przepustowosci z tej sciezki
        while (tmp != s) {
            cout << tmpT << " -> " << tmp << " min: " << min << endl;
            tmpT = tmp;
            tmp = f2[tmp];
            if (min > siecResidualna.tab[tmp][tmpT].remainingFlow) {
                min = siecResidualna.tab[tmp][tmpT].remainingFlow;
            }
        }
        cout << tmpT << " -> " << tmp << " min: " << min << endl;

        tmpT = t;
        tmp = f2[t];
        //aktualizuje siec residualna
        while (tmp != s) {
            siecResidualna.tab[tmp][tmpT].remainingFlow -= min;
            siecResidualna.tab[tmpT][tmp].remainingFlow += min;
            tmpT = tmp;
            tmp = f2[tmp];
        }
        siecResidualna.tab[tmp][tmpT].remainingFlow -= min;
        siecResidualna.tab[tmpT][tmp].remainingFlow += min;

        //dodaje minimum z tej ?cie?ki do wyniku, jako maksymalny przep?yw
        maxFlow1 += min;
    }
    double result = min(maxFlow, maxFlow1);
    //cout << maxFlow << " " << maxFlow1;
    return result;
}

//W trakcie pracy. Jeszcze nie dziala prawidlowo !
//Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
bool Matrix::bfs2(int s, const vector<vector<EdgeData>>& graf, int t, vector<int>& f) {
    vector<int> visited(vertices + 1, 0);
    queue<int> kolejka;

    visited[s] = 1;
    kolejka.push(s);

    while (!kolejka.empty()) {
        int x = kolejka.front();
        kolejka.pop();

        visited[x] = 1;
        for (int u = 1; u <= vertices; u++) {
            if (graf[x][u].remainingFlow > 0 && visited[u] == 0) {
                visited[u] = 1;
                kolejka.push(u);
                f[u] = x;

                if (u == t) {
                    return true;
                }
            }
        }
    }
    return false;
}