
#include "gtest/gtest.h"
#include "Matrix.h"

#include "printNodesRoads.h"
#include "printRoutes.h"

#include <string>
#include <sstream>
#include <functional>

#include <fstream>
#include <windows.h>
#include <locale>


// Konstruktor
Matrix::Matrix() : vertices(0) {
}

Matrix::Matrix(int n) : vertices(n) {
    tab.resize(n, vector<EdgeData>(n));  // Inicjalizuje macierz o rozmiarze n x n, wype?nion? zerami
    listVertices.resize(n);
    source = 0; // Inicjalizuje ?ród?o
    target = n - 1; // Inicjalizuje uj?cie
}

void Matrix::init(int n) {
    if (n <= vertices) {
        // Je?li zmniejszamy lub rozmiar si? nie zmienia, mo?esz zrobi? resize bez kopiowania
        vertices = n;
        tab.resize(vertices);
        for (int i = 0; i < vertices; i++) {
            tab[i].resize(vertices);
        }
    }
    else {
        // Je?li zwi?kszamy rozmiar, zachowujemy stare dane i rozszerzamy macierz
        int oldVertices = vertices;
        vertices = n;

        // Powi?kszamy liczb? wierszy
        tab.resize(vertices);

        // Powi?kszamy ka?dy wiersz (istniej?cy i nowy)
        for (int i = 0; i < vertices; i++) {
            tab[i].resize(vertices);
        }
    }
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            tab[i][j].remainingFlow = tab[i][j].flow;
        }
    }
    source = 0; // Inicjalizuje ?ród?o
    target = n - 1; // Inicjalizuje uj?cie
}

// Funkcja do dodawania kraw?dzi (u, v) z wag? 'weight'
void Matrix::addEdge(int u, int v, double weight, double cost) {
    tab[u][v] = EdgeData(v, weight, cost);  // Dodaje wag? kraw?dzi w macierzy
}

//Dodaje info o Node
void Matrix::addNode(const Node& node) {
    listVertices.push_back(node);
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


//Czyta dane z pliku i inicjalizuje macierz
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

// Remaster wczytywania autorstwa JK
bool Matrix::readFileToGraph3(string fileName) {
    ifstream plik(fileName);
    int vertices=0; // ilosc wierzcholkow
    int u, v;// u , v (u --> v), przepustowosc
    double maxFlow, x, y, capacity, cost = 0.0;
    string line;
    string fieldType;
    int nodeId;
    if (!plik.is_open()) {
        cerr << "Nie mo?na otworzy? pliku!: " << fileName << endl;
        return false;
    }

    // Pomijamy lini? "KONWERSJA"
    getline(plik, line);
    if (line != "KONWERSJA") {
        cerr << "Blad skladniowy na poczatku wczytywania pliku" << endl;
        return false;
    }

    //Wczytuje ilosc wierzcholkow, ilosc krawedzi
    plik >> Node::breweryEfficiencyMultiplier;

    plik.ignore();
    getline(plik, line);

    if (line != "PUNKTY") {
        cerr << "Blad skladniowy we wczytywaniu sekcji KONWERSJA" << endl;
        return false;
    }

    // Wczytywanie Node'w
    while (getline(plik, line)) {
        if (line == "DROGI") break;

        istringstream iss(line);
        iss >> nodeId >> x >> y >> fieldType;

        capacity = -1;
        Node::NodeType type;
        if (fieldType == "pole") type = Node::NodeType::Field;
        else if (fieldType == "browar") {
            type = Node::NodeType::Brewery;
            iss >> capacity;
        }
        else if (fieldType == "karczma") type = Node::NodeType::Pub;
        else if (fieldType == "brak") type = Node::NodeType::None;

        vertices++;
        Node tmpNode(nodeId, x, y, capacity, type);
        addNode(tmpNode);
    }

    if (line != "DROGI") {
        cerr << "Blad skladniowy we wczytywaniu sekcji PUNKTY" << endl;
        return false;
    }

    //inicjalizuje macierz
    vertices += 2;             // abstrakcyjne
    this->init(vertices);

    // Wczytywanie kraw?dzi
    while(true){
        getline(plik, line);
        istringstream iss(line);
        if (iss >> u >> v >> maxFlow >> cost)addEdge(u, v, maxFlow, cost);
        else break;
    }
    if (line != "CWIARTKI") {
        cerr << "Blad skladniowy we wczytywaniu sekcji DROGI" << endl;
        return false;
    }
    // Wczytywanie punktow oraz mnoznikow cwiartek
    vector<Quarter> initialQuarters(4);
    double efficiency;                  // dzielone przez tysiac, aby zrobic mnoznik
    for (int i = 0; i < 4; i++) {
        getline(plik, line);
        istringstream iss(line);
        iss >> efficiency;
        efficiency /= 1000;
        initialQuarters[i].efficiencyMultiplier = efficiency;
        while (iss >> x >> y)initialQuarters[i].pointTab.push_back(make_pair(x, y));
    }

    //sortowanie punktow wzgledem wspolrzednej katowej
    for (int i = 0; i < 4; i++)initialQuarters[i].sort();

    //szczerze nie warto bylo przepisywac cwiartki do klasy bo tu jest syf
    //w generatorze jest porzadeczek :D
    
    //algorytm otoczki wypuklej w celu ustalenia wielokata wypuklego
    vector<Quarter> quarters(4);
    for (int k = 0; k < 4; k++) {
        quarters[k].efficiencyMultiplier = initialQuarters[k].efficiencyMultiplier;
        quarters[k].pointTab.push_back(initialQuarters[k].pointTab[0]);
        quarters[k].pointTab.push_back(initialQuarters[k].pointTab[1]);

        for (int i = 2; i < initialQuarters[k].pointTab.size(); i++) {
            if (Quarter::comparator(quarters[k].pointTab[quarters[k].pointTab.size() - 2], quarters[k].pointTab[quarters[k].pointTab.size() - 1], initialQuarters[k].pointTab[i]))quarters[k].pointTab.push_back(initialQuarters[k].pointTab[i]);
            else {
                quarters[k].pointTab.pop_back();
                while (!Quarter::comparator(quarters[k].pointTab[quarters[k].pointTab.size() - 2], quarters[k].pointTab[quarters[k].pointTab.size() - 1], initialQuarters[k].pointTab[i]))quarters[k].pointTab.pop_back();
                quarters[k].pointTab.push_back(initialQuarters[k].pointTab[i]);
            }
        }
        quarters[k].pointTab.push_back(initialQuarters[k].pointTab[0]);
    }

    //czy to na pewno wielokat?
    for (int i = 0; i < 4; i++)if (quarters[i].pointTab.size() < 3) {
        cerr << "Wielokat " << i + 1 << " nie jest wielokatem, bo ma za malo bokow";
        return false;
    }

    //zmieniamy efektywnosc produkcji zboza przez pola w zaleznosci od tego w jakiej cwiartce sie znajduje
    bool isFieldValid;
    for(int i=0;i<listVertices.size();i++)if(listVertices[i].type==Node::NodeType::Field){
        pair<double, double> fieldCoordinates = make_pair(listVertices[i].x, listVertices[i].y);
        for (int quarterIndex = 0; quarterIndex < 4; quarterIndex++) {
            isFieldValid = true;
            for (int j = 0; j < quarters[quarterIndex].pointTab.size() - 1; j++) {
                if (!Quarter::comparator(quarters[quarterIndex].pointTab[j], quarters[quarterIndex].pointTab[j + 1], fieldCoordinates)) {
                    isFieldValid = false;
                    break;
                }
            }
            if (isFieldValid) {
                tab[0][i+1].flow *= quarters[quarterIndex].efficiencyMultiplier;
                tab[0][i + 1].remainingFlow *= quarters[quarterIndex].efficiencyMultiplier;
                break;
            }
            else if(!isFieldValid && quarterIndex==3){
                cerr << "Pole z indeksem: " << i + 1 << " nie nalezy do zadnego wielokata" << endl;
                return false;
            }
        }
    }

    //UZYWAC JAKBY CWIARTKI NIE DZIALALY
    //for (int i = 0; i < listVertices.size(); i++)if (listVertices[i].type == Node::NodeType::Field)cout << tab[0][i+1].flow << " "<<i<<"->" << tab[0][i+1].v << endl;
    /*
    for (int i = 0; i < 4; i++) {
        for (auto it = quarters[i].pointTab.begin(); it != quarters[i].pointTab.end(); it++)cout << it->first << " " << it->second << endl;
        cout << endl;
    }
    */

    if (!plik.eof()){
        cerr << "Blad skladniowy we wczytywaniu sekcji CWIARTKI" << endl;
        return false;
    }
    plik.close();


    // tworzy krawedzie od pub do ujscia
    const double INF = numeric_limits<double>::max();
    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Pub) {
            tab[i.GetId()][target].flow = INF;
            tab[i.GetId()][target].remainingFlow = INF;
        }
    }
    return true;
}

//Zmodyfikowany bfs.  Je?eli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
bool Matrix::bfs(int s, const vector<vector<EdgeData>>& graf, int t, vector<int>& f) {
    vector<int> visited(f.size(), 0);
    queue<int> kolejka;

    visited[s] = 1;
    kolejka.push(s);

    while (!kolejka.empty()) {
        int x = kolejka.front();
        kolejka.pop();

        visited[x] = 1;
        for (int u = 1; u < f.size(); u++) {
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

//algorytm znajdowania najkrotszych sciezek.
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

//algorytm znajdowania najkrotszych sciezek.
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

//algorytm znajdowania najkrotszych sciezek. Zmodyfikowany pod nasz problem
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
// Nie tworze w nim sieci residualnej. Dzia?a na orginale. Zatem uruchomienie dwukrotnie tej metody nie da dobrych wynikow
double Matrix::BusackerGowen2(double const maxFlow, int s, int t, vector<Path>& roads,
    bool (Matrix::* shortestPathFunc)(int, int, double&, vector<int>&, const vector<vector<EdgeData>>&)) {

    double result = 0; //przeplyw, zwieksza si? a? nie osiagnie warto?ci F- tj. maxFlow. Podanego jako argument funkcji.
    double summaricResult = 0; //Suma (cost * przeplyw)-jedna sciezka
    double sumCost = 0; // Suma kosztow sciezek
    vector<int> f(vertices); //tablica ojcow
    double cost; //koszt jednej sciezki
    vector<int> path; // lista sciezek do wypisanie


    // Wykonuje dopoki nie zostal osiagniety przeplyw F(maxFlow)
    // i dopoki istnieje sciezka z s do t (wybiera t? sci??k? która ma najni?szy koszt)
    while (result < maxFlow && (this->*shortestPathFunc)(s, t, cost, f, tab)) {

        int tmpT = t;
        int tmp = f[t];
        double maxFlowOfPath = tab[tmp][tmpT].remainingFlow;
        while (tmp != s) {
            tmpT = tmp;
            tmp = f[tmp];
            maxFlowOfPath = min(maxFlowOfPath, tab[tmp][tmpT].remainingFlow);
        }

        if (result + maxFlowOfPath > maxFlow) {
            maxFlowOfPath = maxFlow - result;
        }

        //aktualizuje siec residualna
        tmpT = t;
        tmp = f[t];
        while (tmp != s) {
            tab[tmp][tmpT].remainingFlow -= maxFlowOfPath;
            tab[tmpT][tmp].remainingFlow += maxFlowOfPath;
            tab[tmpT][tmp].cost = - tab[tmp][tmpT].cost;
            path.push_back(tmpT);
            tmpT = tmp;
            tmp = f[tmp];
        }
        tab[tmp][tmpT].remainingFlow -= maxFlowOfPath;
        tab[tmpT][tmp].remainingFlow += maxFlowOfPath;
        tab[tmpT][tmp].cost = - tab[tmp][tmpT].cost;
        path.push_back(tmpT);
        path.push_back(s);
        reverse(path.begin(), path.end());

        roads.push_back(Path(path , maxFlowOfPath , cost));
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
void Matrix::maxFlowMinCost() {
    vector<Path> a;
    double maxFlow = edmondsKarp();
    cout << endl;
    BusackerGowen2(maxFlow, 0 , vertices - 1, a , &Matrix::dijkstraModify);
    cout << endl;
    BusackerGowen2(maxFlow, 0, vertices - 1, a , &Matrix::BellmanFord);
}

//minCostMaxFlow dla naszego problemu. Z wykorzystaniem algorytmu BusackeraGowena
double Matrix::maxFlowMinCost2() {
    pair<double,double> maxFlow = maxFlowAlgorithm();
    int midT = vertices;
    this->init(vertices + 1);

    vector<Path> road1;
    vector<Path> road2;


    for (Node i : listVertices) {

        if (i.GetType() == Node::NodeType::Brewery) {
            tab[i.GetId()][midT].remainingFlow = i.GetCapacity();
            tab[i.GetId()][midT].flow = i.GetCapacity();
            tab[i.GetId()][midT].cost = 0;
       }
    }

    double result1 = BusackerGowen2(maxFlow.first, 0, midT, road1, &Matrix::BellmanFord);

    //Resetuje wszystkie powrotne krawedzie ktore nie istnieja w orginalny grafie
    for (int i = 0; i < vertices - 1; i++) {
        for (int j = 0; j < vertices; j++) {
            if (!(tab[i][j].flow > 0)) {
                tab[i][j].remainingFlow = 0;
                tab[i][j].cost = 0;
            }
        }
    }

    //Resetuje wszystkie krawedzie od Browarów do midT 
    //W druga strone zostawiam celowo tj z midT -> browar
    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Brewery) {
            if (tab[i.GetId()][midT].remainingFlow > 0)
                tab[i.GetId()][midT].remainingFlow = 0;
        }
    }

    double result2 = BusackerGowen2(maxFlow.second, midT, vertices - 2, road2, &Matrix::BellmanFord);


    vector<Path> goodRoads1 = Path::filterPaths(road1, this->tab);
    vector<Path> goodRoads2 = Path::filterPaths(road2, this->tab);
    vector<Path> combined = Path::combineRoads(goodRoads1, goodRoads2, this->tab);
    for (Path p : combined) {
        for (int i : p.getPath()) {
            cout << i << " ";
        }
        cout << "Przepustowosc: " << p.getFlow() << " Koszt: " << p.getCost() << endl;
    }

    printToFileSolution(min(maxFlow.first, maxFlow.second), goodRoads1, goodRoads2);

    return result1 + result2;
}

//stara wersja printToFileSolution
/*
//Zapisuje wynik do pliku i wypisuje
void Matrix::printToFileSolution(double maxFlow, vector<Path> firstRoads, vector<Path> secondRoads) {

    wofstream out("wynik.txt", ios::binary);
    out.imbue(locale(out.getloc(),
        new codecvt_utf8<wchar_t>));  // ustawienie UTF-8

    if (!out.is_open()) {
        wcerr << "Nie mo?na otworzy? pliku wynik.txt do zapisu." << endl;
        return;
    }
   // out << (wchar_t)0xFEFF;
    // Wypisanie punktów
    wstring name;
    out << L"Punkty:" << endl;
    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Field) {
            name = L"pole";
        }
        else if (i.GetType() == Node::NodeType::Brewery) {
            name = L"browar";
        }
        else if (i.GetType() == Node::NodeType::Pub) {
            name = L"karczma";
        }
        else {
            name = L"skrzy?owanie";
        }

        out << L"Punkt " << to_wstring(i.GetId()) << L": typ: " << name << L"; pozycja x = " << to_wstring(i.GetX()) << L", y = " << to_wstring(i.GetY());
        if (name == L"pole") {
            double productivity = tab[0][i.GetId()].flow;
            out << L"; wydajno??: " << to_wstring(productivity) << L" ton." << endl;
        }
        else if (name == L"browar") {
            out << L"; pojemno??: " << to_wstring(i.GetCapacity()) << L" ton." << endl;
        }
        else {
            out << L"." << endl;
        }
    }
    //Mapowanie typów w?z?ów
    unordered_map<int, wstring> tab;
    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Field) {
            tab[i.GetId()] = L"pola";
        }
        else if (i.GetType() == Node::NodeType::Brewery) {
            tab[i.GetId()] = L"browaru";
        }
        else if (i.GetType() == Node::NodeType::Pub) {
            tab[i.GetId()] = L"karczmy";
        }
        else {
            tab[i.GetId()] = L"skrzy?owania";
        }
    }

    // Drogi pola -> browar
    int countRoads = 1;
    out << endl << L"Drogi: " << endl;
    for (const Path& p : firstRoads) {
        const vector<int>& path = p.getPath();
        out << L"Droga " << countRoads << L": z pola ";
        for (size_t i = 0; i < path.size(); ++i) {
            int node = path[i];
            if (i == 0) {
                out << L"[" << node << L"] na pozycji (" << listVertices[node - 1].GetX() << L", " << listVertices[node - 1].GetY() << L") ";
            }
            else {
                out << L"do " << tab[node] << L" [" << node << L"] na pozycji (" << listVertices[node - 1].GetX() << L", " << listVertices[node - 1].GetY() << L") ";
            }
        }
        out << L": przepustowo??: " << p.getFlow() << L" ton; koszt naprawy " << p.getCost() << L" srebrnych pensów." << endl;
        countRoads++;
    }
    // Drogi browar -> karczma



    int countRoads2 = countRoads;
    for (const Path& p : secondRoads) {
        const vector<int> path = p.getPath();
        out << L"Droga " << countRoads2 << L": z browaru ";
        for (size_t i = 0; i < path.size(); ++i) {
            int node = path[i];
            if (i == 0) {
                out << L"[" << node << L"] na pozycji (" << listVertices[node - 1].GetX() << L", " << listVertices[node - 1].GetY() << L")";
            }
            else {
                out << L" do " << tab[node] << L" [" << node << L"] na pozycji (" << listVertices[node - 1].GetX() << L", " << listVertices[node - 1].GetY() << L")";
            }
        }
        out << L": przepustowo??: " << p.getFlow() << L" ton; koszt naprawy " << p.getCost() << L" srebrnych pensów." << endl;
        countRoads2++;
    }

    out << endl;
    out << L"Rozwi?zanie:" << endl;
    out << L"Maksymalna ilo?? piwa, któr? mo?na przetransportowa? : " << maxFlow << L" ton" << endl;

    // Po??czenie dróg
    vector<Path> combined = Path::combineRoads(firstRoads, secondRoads, this->tab);
    // Wypisanie trasy ko?cowej
    out << L"Przebieg trasy: " << endl;
    countRoads = 1;
    for (const Path& p : combined) {
        out << L"Trasa nr " << countRoads << L":" << endl;
        out << L"Z pola ";
        const vector<int>& path = p.getPath();
        for (size_t i = 0; i < path.size(); ++i) {
            int node = path[i];
            if (i == 0) {
                out << L"[" << node << L"] na pozycji (" << listVertices[node - 1].GetX() << L", " << listVertices[node - 1].GetY() << L") " << p.getFlow() << L" ton ";
            }
            else {
                out << L"z " << tab[node] << L" [" << node << L"] na pozycji (" << listVertices[node - 1].GetX() << L", " << listVertices[node - 1].GetY() << L") " << p.getFlow() << L" ton ";
            }
        }
        countRoads++;
        out << "\n";
    }
    out.close();
    //WYPISYWANIE
    wifstream file(L"wynik.txt");
    file.imbue(locale(file.getloc(), new codecvt_utf8<wchar_t>()));
    if (!file.is_open()) {
        wcerr << L"Nie mo?na otworzy? pliku wynik.txt" << endl;
    }
    else {
        wstring line;

        while (getline(file, line)) {
            wcout << line << L"\n";
        }

        file.close();
    }
}
*/




//Zapisuje wynik do pliku i wypisuje
void Matrix::printToFileSolution(double maxFlow, vector<Path> firstRoads, vector<Path> secondRoads) {

    wofstream out("wynik.txt", ios::binary);
    out.imbue(locale(out.getloc(),
        new codecvt_utf8<wchar_t>));  // ustawienie UTF-8

    if (!out.is_open()) {
        wcerr << "Nie mozna otworzyc pliku wynik.txt do zapisu." << endl;
        return;
    }
    // out << (wchar_t)0xFEFF;


    //wypisanie punktów
    printNodes(out, listVertices);


    //wypisanie dróg
    printRoads(out, tab, listVertices);





    out << endl;
    out << L"Rozwi¹zanie:" << endl;
    out << L"Maksymalna iloœæ piwa, któr¹ mo¿na przetransportowaæ: " << maxFlow << L" ton;" << endl;
    out << L" koszt naprawy: " << endl;                                                     ///////// !!



    out.close();


    //WYPISYWANIE
    wifstream file(L"wynik.txt");
    file.imbue(locale(file.getloc(), new codecvt_utf8<wchar_t>()));
    if (!file.is_open()) {
        wcerr << L"Nie mo?na otworzy? pliku wynik.txt" << endl;
    }
    else {
        wstring line;

        while (getline(file, line)) {
            wcout << line << L"\n";
        }

        file.close();
    }
}


// Klasyczny algorytm Edmondsa-Karpa
double Matrix::edmondsKarp() {
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
    cout << "Maksymalny przeplyw" << maxFlow;
    return maxFlow;
}

// Klasyczny algorytm Edmondsa-Karpa. Z podaniem argumentow
double Matrix::edmondsKarpClassic(int s, vector<vector<EdgeData>>& graf, int t, vector<int>& f) {
    double maxFlow = 0;
    while (bfs(s, graf, t, f))
    {
        int tmpT = t;
        int tmp = f[t];
        cout << " Droga: z " << s << " do " << t << endl;
        double min = graf[tmp][tmpT].remainingFlow;

        //Wypisuje siezki powieszajace i wyznacza minimum przepustowosci z tej sciezki
        while (tmp != s) {
            cout << tmpT << " -> " << tmp << " min: " << min << endl;
            tmpT = tmp;
            tmp = f[tmp];
            if (min > graf[tmp][tmpT].remainingFlow) {
                min = graf[tmp][tmpT].remainingFlow;
            }
        }
        cout << tmpT << " -> " << tmp << " min: " << min << endl;

        tmpT = t;
        tmp = f[t];
        //aktualizuje siec residualna
        while (tmp != s) {
            graf[tmp][tmpT].remainingFlow -= min;
            graf[tmpT][tmp].remainingFlow += min;
            tmpT = tmp;
            tmp = f[tmp];
        }
        graf[tmp][tmpT].remainingFlow -= min;
        graf[tmpT][tmp].remainingFlow += min;
        //dodaje minimum z tej ?cie?ki do wyniku, jako maksymalny przep?yw
        maxFlow += min;
    }
    cout << "maksymalny przelyw:" << maxFlow << endl;
    return maxFlow;
}

//Stary "edmonsKarp2()". Liczy maksymalny przeplyw dla naszego problemu. Zwraca dwa przep?ywy.
pair<double, double>  Matrix::maxFlowAlgorithm() {
    int s = 0;
    int t = vertices - 1;
    const double INF = numeric_limits<double>::max();
    int midT = vertices; // sztuczny wierzcho?ek
    double maxFlow = 0, maxFlow1 = 0, result = 0; //maksymalny przeplyw
    Matrix siecResidualna(vertices + 1); //siec residualna

    //Siec residualna na start algorytmu jest taka sama jak siec przep?ywowa
    // powieksza sie? o 1 (dodaje midT)
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

    // Od kazdego wierzcholka ktory jest browarem. Tworzy nowa krawedz do midT
    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Brewery) {
            siecResidualna.tab[i.GetId()][midT].remainingFlow = i.GetCapacity();
        }
    }
    vector<int> f(vertices + 1); //tablica ojcow dla 1 przeplywu
    vector<int> f2(vertices + 1); //tablica ojcow dla 2 przeplywu

    maxFlow = edmondsKarpClassic(s, siecResidualna.tab, midT, f);

    //Resetuje wszystkie powrotne krawedzie ktore nie istnieja w orginalny grafie
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (!(tab[i][j].flow > 0)) {
               siecResidualna.tab[i][j] = EdgeData();
            }
        }
    }

    //Resetuje wszystkie krawedzie od Browarów do midT 
    //W druga strone zostawiam celowo tj z midT -> browar
    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Brewery) {
            if (siecResidualna.tab[i.GetId()][midT].remainingFlow > 0)
                siecResidualna.tab[i.GetId()][midT].remainingFlow = 0;
               
        }
    }

    maxFlow1 = edmondsKarpClassic(midT, siecResidualna.tab, t, f);

    return { maxFlow, maxFlow1 };
}

