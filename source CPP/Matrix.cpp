#include "Matrix.h"
//#include "../FordFulkersonTests/pch.h"
//#include "CppUnitTest.h"

// Konstruktor
Matrix::Matrix() : vertices(0) {
}

Matrix::Matrix(int n) : vertices(n) {
    tab.resize(n, vector < double > (n, 0));  // Inicjalizuje macierz o rozmiarze n x n, wype?nion? zerami
}

void Matrix::init(int n) {
    vertices = n;
    tab.resize(n, vector < double >(n, 0));
}

// Funkcja do dodawania kraw?dzi (u, v) z wag? 'weight'
void Matrix::addEdge(int u, int v, double weight) {
        tab[u][v] = weight;  // Dodaje wag? kraw?dzi w macierzy
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
            cout << tab[i][j] << " ";
        }
        cout << endl;
    }
}

bool Matrix::readFileToGraph(string fileName) {
    ifstream plik(fileName);
    int vertices; // ilosc wierzcholkow
    int newEdge1, newEdge2, edges;// u , v (u --> v), przepustowosc, ilosc krawedzi 
    double maxFlow;
    string line;
    int nodeId, x, y;
    double capacity;

    if (!(plik >> vertices >> edges))
    {
        cerr << "Nie mo?na otworzy? pliku ";
        return false;
    }
    //inicjalizuje macierz
    this->init(vertices);

    //wczytuje krawedzie
    for (int i = 0; i < edges; i++) {    
        plik >> newEdge1 >> newEdge2 >> maxFlow;
        addEdge(newEdge1, newEdge2, maxFlow);
    }

    // Wczytywanie Node'ów
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

//Klasyczny fordFulkersow
double Matrix::fordFulkerson() {
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
        double min = siecResidualna.tab[tmp][tmpT];

        //Wypisuje siezki powieszajace i wyznacza minimum przepustowosci z tej sciezki
        while (tmp != s) {
           //cout << tmpT << " -> " << tmp << " min: " << min << endl;
            tmpT = tmp;
            tmp = f[tmp];
            if (min > siecResidualna.tab[tmp][tmpT]) {
                min = siecResidualna.tab[tmp][tmpT];
            }
        }
        //cout << tmpT << " -> " << tmp << " min: " << min << endl;

        tmpT = t;
        tmp = f[t];
        //aktualizuje siec residualna
        while (tmp != s) {
            siecResidualna.tab[tmp][tmpT] -= min;
            siecResidualna.tab[tmpT][tmp] += min;
            tmpT = tmp;
            tmp = f[tmp];
        }
        siecResidualna.tab[tmp][tmpT] -= min;
        siecResidualna.tab[tmpT][tmp] += min;

        //dodaje minimum z tej ?cie?ki do wyniku, jako maksymalny przep?yw
        maxFlow += min;
    }
    return maxFlow;
}

//Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
bool Matrix::bfs(int s, const vector<vector<double>>& graf, int t, vector<int>& f) {
    vector<int> visited(vertices, 0);
    queue<int> kolejka;

    visited[s] = 1;
    kolejka.push(s);

    while (!kolejka.empty()) {
        int x = kolejka.front();
        kolejka.pop();

        visited[x] = 1;
        for (int u = 1; u < vertices; u++) {
            if (graf[x][u] > 0 && visited[u] == 0) {
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



//W trakcie pracy. Jeszcze nie dziala prawidlowo ! 
////Ford Fulkerson dla sieci w ktorej interesuja nas sciezki powiekszajace s-> ... -> t 
// (Gdzie gdzies po drodze jest wierzcholek ktory jest browarem)
double Matrix::fordFulkerson2() {
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

    while (bfs2(s, siecResidualna.tab, t, f, this->listVertives))
    {
        int tmpT = t;
        int tmp = f[t];
        // cout << " Droga: z " << s << " do " << t << endl;
        double min = siecResidualna.tab[tmp][tmpT];

        //Wypisuje siezki powieszajace i wyznacza minimum przepustowosci z tej sciezki
        while (tmp != s) {
            //cout << tmpT << " -> " << tmp << " min: " << min << endl;
            tmpT = tmp;
            tmp = f[tmp];
            if (min > siecResidualna.tab[tmp][tmpT]) {
                min = siecResidualna.tab[tmp][tmpT];
            }
        }
        //cout << tmpT << " -> " << tmp << " min: " << min << endl;

        tmpT = t;
        tmp = f[t];
        //aktualizuje siec residualna
        while (tmp != s) {
            siecResidualna.tab[tmp][tmpT] -= min;
            siecResidualna.tab[tmpT][tmp] += min;
            tmpT = tmp;
            tmp = f[tmp];
        }
        siecResidualna.tab[tmp][tmpT] -= min;
        siecResidualna.tab[tmpT][tmp] += min;

        //dodaje minimum z tej ?cie?ki do wyniku, jako maksymalny przep?yw
        maxFlow += min;
    }
    return maxFlow;
}

//W trakcie pracy. Jeszcze nie dziala prawidlowo !
//Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t (ale przechodzi przez browar!) to zwraca true. Aktulizuje tez tablice ojcow
bool Matrix::bfs2(int s, const vector<vector<double>>& graf, int t, vector<int>& f, const vector<Node> type) {
    vector<int> visited(vertices, 0);
    queue<int> kolejka;

    visited[s] = 1;
    kolejka.push(s);
    bool ifKarczmaVisited;

    while (!kolejka.empty()) {
        int x = kolejka.front();
        kolejka.pop();
        visited[x] = 1;
        for (int u = 1; u < vertices; u++) {
            if (graf[x][u] > 0 && visited[u] == 0) {
                visited[u] = 1;
                kolejka.push(u);
                f[u] = x;
                if (u >= 0 && u < listVertives.size()) {
                    if (listVertives[u].type == Node::NodeType::Brewery) {
                        cout << listVertives[u].id;
                        ifKarczmaVisited = true;
                    }
                }
                if (u == t && ifKarczmaVisited) {
                    return true;
                }
            }
        }
    }
    return false;
}