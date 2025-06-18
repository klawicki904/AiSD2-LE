
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
#include "AdjacencyList.h"


// Konstruktor
Matrix::Matrix() : vertices(0) {
}

Matrix::Matrix(int n) : vertices(n) {
    tab.resize(n, vector<EdgeData>(n));  // Inicjalizuje macierz o rozmiarze n x n, wype?nion? zerami
    listVertices.resize(n);
    source = 0; // Inicjalizuje ?r?d?o
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
        // Jesli zwiekszamy rozmiar, zachowujemy stare dane i rozszerzamy macierz
        int oldVertices = vertices;
        vertices = n;

        // Powiekszamy liczbe wierszy
        tab.resize(vertices);

        // Powiekszamy kazdy wiersz (istniejacy i nowy)
        for (int i = 0; i < vertices; i++) {
            tab[i].resize(vertices);
        }
    }
    for (int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            tab[i][j].remainingFlow = tab[i][j].flow;
        }
    }
    source = 0; // Inicjalizuje zrodlo
    target = n - 1; // Inicjalizuje ujscie
}

// Funkcja do dodawania krawedzi (u, v) z waga 'weight'
void Matrix::addEdge(int u, int v, double weight, double cost) {
    tab[u][v] = EdgeData(v, weight, cost);  // Dodaje wagi krawedzi w macierzy
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
        cerr << "Nie mozna otworzyc pliku ";
        return false;
    }
    //inicjalizuje macierz
    this->init(vertices);

    // Wczytywanie krawedzi
    for (int i = 0; i < edges; i++) {
        getline(plik, line);
        if (line.empty()) {
            i--; // ignoruj puste linie
            continue;
        }

        istringstream iss(line);

        if (!(iss >> u >> v >> maxFlow)) {
            std::cerr << "Blad podczas wczytywania krawedzi w linii: " << line << std::endl;
            continue;
        }

        if (!(iss >> cost)) {
            cost = 0.0; // Jesli brak kosztu, przypisz domyslnie
        }

        addEdge(u, v, maxFlow, cost);
    }


    // Wczytywanie Node'ow
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
        cerr << "Nie mozna otworzyc pliku!: " << fileName << endl;
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
    while (getline(plik, line))
    {
        if (line.empty()) { continue; } // pomijaj puste linie
        break; // przerywa petla jak znajdzie niepusta linie
    }

    // Wczytywanie Node'ow
    while (getline(plik, line)) {
        if (line.empty()) { continue; } // pomijaj puste linie
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

    // Wczytywanie krawedzi
    while (getline(plik, line)) {
        if (line.empty()) { continue; } // pomijaj puste linie
        istringstream iss(line);
        if (iss >> u >> v >> maxFlow >> cost)addEdge(u, v, maxFlow, cost);
        else break;
    }
    if (line != "CWIARTKI") {
        cerr << "Blad skladniowy we wczytywaniu sekcji DROGI" << endl;
        return false;
    }
    // Wczytywanie punktow oraz mnoznikow cwiartek
    vector<Quarter> initialQuarters;
    size_t quartersCount = 0;
    Quarter tempQuarter;
    double efficiency;
    while (getline(plik, line)) {
        if (line.empty()) { continue; } // pomijaj puste linie
        cout << "line: " << line << endl;
        istringstream iss(line);
        iss >> efficiency;
        cout << "eff: " << efficiency << endl;
        //efficiency /= 1000;
        tempQuarter.efficiencyMultiplier = efficiency;
        initialQuarters.push_back(tempQuarter);

        while (iss >> x >> y)
        {
            cout << "x; y: " << x << "; " << y << endl;
            initialQuarters[quartersCount].pointTab.push_back(make_pair(x, y));
        }
        quartersCount++;
        cout << "quartersCount: " << quartersCount << endl;
    }
    this->initialQuarters = initialQuarters;
    //sortowanie punktow wzgledem wspolrzednej katowej
    for (int i = 0; i < quartersCount; i++)initialQuarters[i].sort();

    //szczerze nie warto bylo przepisywac cwiartki do klasy bo tu jest syf
    //w generatorze jest porzadeczek :D
    
    //algorytm otoczki wypuklej w celu ustalenia wielokata wypuklego
    vector<Quarter> quarters(quartersCount);
    for (int k = 0; k < quartersCount; k++){
        quarters[k].efficiencyMultiplier = initialQuarters[k].efficiencyMultiplier;
        quarters[k].pointTab.push_back(initialQuarters[k].pointTab[0]);
        quarters[k].pointTab.push_back(initialQuarters[k].pointTab[1]);
        for (int i = 2; i < initialQuarters[k].pointTab.size(); i++) {
            //cout << initialQuarters[k].pointTab[i].first << " " << initialQuarters[k].pointTab[i].second << endl;
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
    for (int i = 0; i < quartersCount; i++)if (quarters[i].pointTab.size() < 3) {
        cerr << "Wielokat " << i + 1 << " nie jest wielokatem, bo ma za malo bokow";
        return false;
    }
    
    //czy wielokaty sa rozlaczne?
    bool isQuarterValid = false;
    for (int currentQuarterIndex = 0; currentQuarterIndex < quartersCount; currentQuarterIndex++){
        for (int i = 0; i < quarters[currentQuarterIndex].pointTab.size() - 1; i++) {
            for (int quarterIndex = 0; quarterIndex < quartersCount; quarterIndex++) {
                isQuarterValid = false;
                if (quarterIndex == currentQuarterIndex)continue;
                for (int j = 0; j < quarters[quarterIndex].pointTab.size() - 1; j++) {
                    if (!Quarter::comparator(quarters[quarterIndex].pointTab[j], quarters[quarterIndex].pointTab[j + 1], quarters[currentQuarterIndex].pointTab[i])) {
                        isQuarterValid = true;
                        break;
                    }
                }
                if (!isQuarterValid) {
                    cerr << "Pewne dwie cwiartki nie sa ze soba rozlaczne!" << endl;
                    return false;
                }
            }
        }
    }

    //zmieniamy efektywnosc produkcji zboza przez pola w zaleznosci od tego w jakiej cwiartce sie znajduje
    bool isFieldValid;
    for (int i = 0;i < listVertices.size();i++)if (listVertices[i].type == Node::NodeType::Field) {
        pair<double, double> fieldCoordinates = make_pair(listVertices[i].x, listVertices[i].y);
        for (int quarterIndex = 0; quarterIndex < quartersCount; quarterIndex++) {
            isFieldValid = true;
            for (int j = 0; j < quarters[quarterIndex].pointTab.size() - 1; j++) {
                if (!Quarter::comparator(quarters[quarterIndex].pointTab[j], quarters[quarterIndex].pointTab[j + 1], fieldCoordinates)) {
                    isFieldValid = false;
                    break;
                }
            }
            if (isFieldValid) {
 /*             tab[0][i + 1].flow += quarters[quarterIndex].efficiencyMultiplier;
                tab[0][i + 1].remainingFlow += quarters[quarterIndex].efficiencyMultiplier;
                tab[0][i + 1].cost = 0;*/
                this->listVertices[i].capacity = quarters[quarterIndex].efficiencyMultiplier;
                break;
            }
            else if (!isFieldValid && quarterIndex == quartersCount-1) {
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
    //const double INF = numeric_limits<double>::max();
    //for (Node i : listVertices) {
    //    if (i.GetType() == Node::NodeType::Pub) {
    //        tab[i.GetId()][target].flow = INF;
    //        tab[i.GetId()][target].remainingFlow = INF;
    //        tab[i.GetId()][target].cost = 0;
    //    }
    //}
    return true;
}

//Tworzy sie? residualn? z?o?on? z 2 warstwa
void Matrix::createResidualNet(vector<vector<EdgeData>>& graf , const vector<Node>& listNodes) {
    int orginalSize = graf.size() - 2;
    this->init((orginalSize * 2) + 2);

    const double INF = numeric_limits<double>::max();


    for (const Node& i : listNodes) {
        if (i.GetType() == Node::NodeType::Pub) {
            this->tab[i.GetId() + orginalSize][target].flow = INF;
            this->tab[i.GetId() + orginalSize][target].remainingFlow = INF;
            this->tab[i.GetId() + orginalSize][target].cost = 0;
        }
    }

    for (const Node& i : listNodes) {
        if (i.GetType() == Node::NodeType::Field) {
            this->tab[0][i.GetId()].flow = i.GetCapacity();
            this->tab[0][i.GetId()].remainingFlow = i.GetCapacity();
            this->tab[0][i.GetId()].cost = 0;
        }
    }

    for (const Node& i : listNodes) {
        if (i.GetType() == Node::NodeType::Brewery) {
            this->tab[i.GetId()][i.GetId() + orginalSize].flow = i.GetCapacity();
            this->tab[i.GetId()][i.GetId() + orginalSize].remainingFlow = i.GetCapacity();
            this->tab[i.GetId()][i.GetId() + orginalSize].cost = 0;
        }
    }

    //Siec residualna na start algorytmu jest taka sama jak siec przeplywowa
    // powieksza siec o 1 (dodaje midT)
    for (int i = 1; i <= orginalSize; i++) {
        for (int j = 1; j <= orginalSize; j++) {
            if (graf[i][j].flow > 0) {
                this->tab[i][j] = graf[i][j];
                this->tab[i + orginalSize][j + orginalSize] = graf[i][j];
            }
        }
    }
}

//Zmodyfikowany bfs.  Jezeli jest sciezka powiekszajaca z s do t, to zwraca true. Aktulizuje tez tablice ojcow
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

    // Kolejka priorytetowa - pary {odleglosc, wierzcholek}
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;
    pq.push({ 0, source });

    while (!pq.empty()) {
        double dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = 1;

        if (u == target) break;// znaleziono najkrotsza sciezke do t

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

    // Wyswietlenie wynikow
    //cout << "Odleglosci od wierzcholka " << source << ":\n";

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

    Matrix siecResidualna(vertices); //siec residualna

    //Siec residualna na start algorytmu jest taka sama jak siec przep?ywowa
    // powieksza sie? o 1 (dodaje midT)
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if ((j < vertices) && (i < vertices)) {
                siecResidualna.tab[i][j] = tab[i][j];
            }
            else {
                siecResidualna.tab[i][j] = EdgeData();
            }
        }
    }


    // Wykonuje dopoki nie zostal osiagniety przeplyw F(maxFlow)
    // i dopoki istnieje sciezka z s do t (wybiera t? scie?k? kt?ra ma najni?szy koszt)
    while (result < maxFlow && (this->*shortestPathFunc)(s, t, cost, f, siecResidualna.tab)) {
        int tmpT = t;
        int tmp = f[t];
        double maxFlowOfPath = siecResidualna.tab[tmp][tmpT].remainingFlow;
        while (tmp != s) {
            tmpT = tmp;
            tmp = f[tmp];
            maxFlowOfPath = min(maxFlowOfPath, siecResidualna.tab[tmp][tmpT].remainingFlow);
        }

        if (result + maxFlowOfPath > maxFlow) {
            maxFlowOfPath = maxFlow - result;
        }

        //aktualizuje siec residualna
        tmpT = t;
        tmp = f[t];
        while (tmp != s) {
            siecResidualna.tab[tmp][tmpT].remainingFlow -= maxFlowOfPath;
            siecResidualna.tab[tmpT][tmp].remainingFlow += maxFlowOfPath;
            siecResidualna.tab[tmpT][tmp].cost = -siecResidualna.tab[tmp][tmpT].cost;
            path.push_back(tmpT);
            tmpT = tmp;
            tmp = f[tmp];
        }
        siecResidualna.tab[tmp][tmpT].remainingFlow -= maxFlowOfPath;
        siecResidualna.tab[tmpT][tmp].remainingFlow += maxFlowOfPath;
        siecResidualna.tab[tmpT][tmp].cost = -siecResidualna.tab[tmp][tmpT].cost;
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
    cout << "Suma kosztow drog: " << sumCost << endl;

    //wyjatek
    if (result != maxFlow) {
        cout << "UWAGA! Nie osiagnieto maksymalnego przeplywu. GDZIES JEST BLAD";
    }

    return summaricResult;
}



// Klasyczny minCostMaxFlow. Z wykorzystaniem algorytmu BusackeraGowena
void Matrix::maxFlowMinCost() {
    vector<Path> a;
    double maxFlow = edmondsKarp();
    cout << endl;
    BusackerGowen2(maxFlow, source , target, a , &Matrix::dijkstraModify);
    cout << endl;
    BusackerGowen2(maxFlow, source, target, a , &Matrix::BellmanFord);
}

// ROZWIAZANIE ANALIZATORA PRZEPLYWU - nowe
// Idea to przygotowanie grafu w ten sposób, aby uruchamia? algorytmy na jednym grafie zamiast na dwóch
//minCostMaxFlow dla naszego problemu. Z wykorzystaniem algorytmu BusackeraGowena. 
double Matrix::maxFlowMinCost3(string outputPath) {
    cout << endl;
    double result = 0; // Zwraca sumaryczny koszt (mo?e by? przydatne dla testów jednostkowych)
    Matrix siecResidualna(vertices);
    vector<Path> road , resultPaths; //roads - drogi z aglgorytmu. 
    //resultPaths - przefiltrowane drogi bez nodes s i t. Bez nodes z warstwy (zostaj? zmienione na orginalne)
    // i bez duplikacji wierzcho?ka odpowiadaj?cego za browar który przetworzy?. Bo jest to miejsce przej?cia mi?dzy warstwami (wi?c wyyst?puje dwuktrotnie)

    //Tworzy sie? residualn? z?o?on? z 2 warstwa jako jeden graf
    siecResidualna.createResidualNet(tab, listVertices);

    //Oblicza max flow dla tej sieci residualne
    double maxFlow =siecResidualna.edmondsKarpClassicWithConversion(siecResidualna.source, siecResidualna.target, Node::breweryEfficiencyMultiplier , vertices - 2);

    //Po obliczeniu max flow tworzymy na now? orginaln? sie? residualn?
    siecResidualna.createResidualNet(tab, listVertices);

    //uruchamiamy metode na minimalny koszt przy maksymalnym przep?ywie
    result = siecResidualna.BusackerGowen3(maxFlow, siecResidualna.source, siecResidualna.target, road, Node::breweryEfficiencyMultiplier ,vertices - 2, &Matrix::BellmanFord);


    cout << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Drogi dla calego grafu: " << endl;

    //uruchamia metode filtrowania dróg
    resultPaths = Path::filterRoads(road, vertices);

    for (Path p : resultPaths) {
        for (int i : p.getPath()) {
            cout << i << " ";
        }
        cout << "Przepustowosc calej drogi: " << p.getFlow() << " Koszt calej drogi: " << p.getCost() << endl;
        cout << "Punkt ??cz?cy (browar ktory przetworzyl): " << p.getConnectedPoint() << endl;
    }
    //cout << "Koszt wszystkich drog (naprawiana kazda krawedz tylko raz): " << Path::sumUniqueEdgesCost(road, tab) << endl;
    cout << "Maksymalny przeplyw calego grafu : " << maxFlow << endl;
    cout << endl;
    cout << "-------------------------------------------------" << endl;
    cout << endl;

    //Zapisuj? wynik do pliku
    printToFileSolution2(maxFlow, resultPaths, outputPath);

    return result;
}

//STARE ROZWIAZANIE
double Matrix::maxFlowMinCost2(string outputPath) {
  const double INF = numeric_limits<double>::max();
  for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Field) {
            this->tab[0][i.GetId()].flow = i.GetCapacity();
            this->tab[0][i.GetId()].remainingFlow = i.GetCapacity();
            this->tab[0][i.GetId()].cost = 0;
        }
    }

    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Pub) {
            this->tab[i.GetId()][target].flow = INF;
            this->tab[i.GetId()][target].remainingFlow = INF;
            this->tab[i.GetId()][target].cost = 0;
        }
    }

    cout << endl;
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

    //Resetuje wszystkie krawedzie od Browarow do midT 
    //W druga strone zostawiam celowo tj z midT -> browar
    //W druga strone ustawiam na maxFlow(I czesci)
    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Brewery) {
            if (tab[i.GetId()][midT].remainingFlow > 0)
                tab[i.GetId()][midT].remainingFlow = 0;
                tab[midT][i.GetId()].remainingFlow = maxFlow.first;
        }
    }

    double result2 = BusackerGowen2(maxFlow.second, midT, vertices - 2, road2, &Matrix::BellmanFord);

    cout << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Drogi dla calego grafu: " << endl;

    vector<Path> goodRoads1 = Path::filterPaths(road1, this->tab);
    vector<Path> goodRoads2 = Path::filterPaths(road2, this->tab);
    vector<Path> combined = Path::combineRoads(goodRoads1, goodRoads2, this->tab);

    double resultFlow = Path::sumFlow(combined);
    for (Path p : combined) {
        for (int i : p.getPath()) {
            cout << i << " ";
        }
        cout << "Przepustowosc calej drogi: " << p.getFlow() << " Koszt calej drogi: " << p.getCost() << endl;
        cout << "Punkt ??cz?cy (browar ktory przetworzyl): " << p.getConnectedPoint() << endl;
    }

    cout << "Koszt wszystkich drog (naprawiana kazda krawedz tylko raz): " << Path::sumUniqueEdgesCost(combined, tab) << endl;
    cout << "Maksymalny przeplyw calego grafu (nie uwzgledniana jeszcze konwersja): " << resultFlow << endl;
    cout << endl;
    cout << "-------------------------------------------------" << endl;
    cout << endl;

    printToFileSolution2(resultFlow, combined, outputPath);
   // printToFileSolution(min(maxFlow.first, maxFlow.second));

    return result1 + result2;
}

//Zapisuje wynik do pliku i wypisuje
//void Matrix::printToFileSolution2(double maxFlow, vector<Path> combined, string outputPath) {
//
//    wofstream out(outputPath, ios::binary);
//    out.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
//    if (!out.is_open()) {
//        wcerr << "Nie mo?na otworzy? pliku wynik.txt do zapisu." << endl;
//        return;
//    }
//    out << (wchar_t)0xFEFF;
//
//     // Wypisanie punktow
//    wstring name;
//    out << L"Konwersja:" << endl;
//    out << Node::breweryEfficiencyMultiplier << L" ton to ilo?? piwa, jak? uzyskuje si? z tony j?czmienia w ka?dym z browar?w." << endl << endl;
//    out << L"Punkty:" << endl;
//    for (Node& i : listVertices) {
//        if (i.GetType() == Node::NodeType::Field) {
//            name = L"pole";
//        }
//        else if (i.GetType() == Node::NodeType::Brewery) {
//            name = L"browar";
//        }
//        else if (i.GetType() == Node::NodeType::Pub) {
//            name = L"karczma";
//        }
//        else {
//            name = L"skrzy?owanie";
//        }
//
//
//        out << L"Punkt " << i.GetId() << L": typ: " << name << L"; pozycja x = " << i.GetX() << L", y = " << i.GetY();
//        if (name == L"pole") {
//            i.setCapacity(tab[0][i.GetId()].flow);
//            out << L"; wydajno??: " << i.GetCapacity() << L" ton." << endl;
//        }
//        else if (name == L"browar") {
//            out << L"; pojemno??: " << i.GetCapacity() << L" ton." << endl;
//        }
//        else {
//            out << L"." << endl;
//        }
//    }
//
//
//    printRoadsMatrix(out, this->tab, listVertices , vertices - 2);
//    out << endl;
//    out << L"?wiartki:" << endl;
//
//    for (const Quarter& q : initialQuarters) {
//        out << L"warto??: "<< q.efficiencyMultiplier << L"; punkty graniczne: ";
//        for (const auto& point : q.pointTab) {
//            out << L"(" << point.first << L"; " << point.second << L"), ";
//        }
//        out << endl;
//    }
//    
//    //Mapowanie typ?w w?z??w
//    unordered_map<int, wstring> tab;
//    for (Node i : listVertices) {
//        if (i.GetType() == Node::NodeType::Field) {
//            tab[i.GetId()] = L"pola";
//        }
//        else if (i.GetType() == Node::NodeType::Brewery) {
//            tab[i.GetId()] = L"browaru";
//        }
//        else if (i.GetType() == Node::NodeType::Pub) {
//            tab[i.GetId()] = L"karczmy";
//        }
//        else {
//            tab[i.GetId()] = L"skrzy?owania";
//        }
//    }
//
//    //wypisanie punkt?w
////    printNodes(out, listVertices);
//
//
//    out << endl;
//    out << L"Rozwi?zanie:" << endl;
//    out << L"Maksymalna ilo?? piwa, kt?r? mo?na przetransportowa?: " << maxFlow << L" ton; koszt naprawy: ";
//    out << Path::sumUniqueEdgesCost(combined, this->tab) << " srebrnych pens?w" << endl;
//
//    // Wypisanie trasy ko?cowej
//    int countRoads = 1;
//    out << L"Przebieg trasy: " << endl;
//    countRoads = 1;
//    for (const Path& p : combined) {
//
//        int breweryID = p.getConnectedPoint();
//        bool checkBrewery = false;
//
//        out << L"Trasa nr " << countRoads++ << L":" << endl;
//        const vector<int>& path = p.getPath();
//        for (size_t i = 0; i < path.size() - 1; ++i) {
//            int node = path[i];
//            wstring stringPole;
//            if (i == 0) {
//                wstringstream ss;
//                ss << L"zawarto?? pola [" << to_wstring(node) << L"] na pozycji (" << listVertices[node - 1].GetX() << L"; " << listVertices[node - 1].GetY() << L") wynosi " << listVertices[node - 1].GetCapacity() - p.getFlow() << L";" << endl;
//                stringPole = ss.str();
//                listVertices[node - 1].setCapacity(listVertices[node - 1].GetCapacity() - p.getFlow());
//
//
//            }
//
//            out << L"Z " << tab[node] << L" [" << to_wstring(node) << L"] na pozycji (" << listVertices[node - 1].GetX() << L"; " << listVertices[node - 1].GetY() << L") " << p.getFlow() << L" ton " << (checkBrewery ? L"piwa " : L"j?czmienia ");
//            if (path[i + 1] == breweryID) {
//                checkBrewery = true;
//                out << L"do " << tab[path[i + 1]] << L" [" << to_wstring(path[i + 1]) << L"] na pozycji (" << listVertices[path[i + 1] - 1].GetX() << L"; " << listVertices[path[i + 1] - 1].GetY() << L");" << endl;
//                if (i == 0) out << stringPole;
//                out << L"pojemno?? browaru [" << path[i + 1] << L"] na pozycji (" << listVertices[path[i + 1] - 1].GetX() << L"; " << listVertices[path[i + 1] - 1].GetY() << L") wynosi " << (listVertices[path[i + 1] - 1].GetCapacity() - p.getFlow()) << L";" << endl;
//                listVertices[path[i + 1] - 1].setCapacity(listVertices[path[i + 1] - 1].GetCapacity() - p.getFlow());
//            }
//            else {
//                out << L"do " << tab[path[i + 1]] << L" [" << to_wstring(path[i + 1]) << L"] na pozycji (" << listVertices[path[i + 1] - 1].GetX() << L"; " << listVertices[path[i + 1] - 1].GetY() << L");" << endl;
//                if (i == 0) out << stringPole;
//            }
//        }
//    }
//
//            out.close();
//            //WYPISYWANIE
//            wifstream file(L"wynik.txt", ios::binary);
//            file.imbue(locale(file.getloc(), new codecvt_utf8<wchar_t>()));
//            if (!file.is_open()) {
//                wcerr << L"Nie mozna otworzyc pliku wynik.txt" << endl;
//            }
//            else {
//                wstring line;
//                wchar_t firstChar = file.peek();
//                if (firstChar == 0xFEFF) {
//                    file.get(); // zignoruj BOM
//                }
//
//                while (getline(file, line)) {
//                    wcout << line << L"\n";
//                }
//
//                file.close();
//            }
//        }

//Zapisuje wynik do pliku i wypisuje
void Matrix::printToFileSolution2(double maxFlow, vector<Path> combined, string outputPath) {

    wofstream out(outputPath, ios::binary);
    out.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    if (!out.is_open()) {
        wcerr << "Nie mo?na otworzy? pliku wynik.txt do zapisu." << endl;
        return;
    }
    out << (wchar_t)0xFEFF;

     // Wypisanie punktow
    wstring name;
    out << L"Konwersja:" << endl;
    out << Node::breweryEfficiencyMultiplier << L" ton to iloœæ piwa, jak¹ uzyskuje siê z tony jêczmienia w ka¿dym z browarów." << endl << endl;
    out << L"Punkty:" << endl;
    for (const Node& i : listVertices) {
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
            name = L"skrzy¿owanie";
        }


        out << L"Punkt " << i.GetId() << L": typ: " << name << L"; pozycja x = " << i.GetX() << L", y = " << i.GetY();
        if (name == L"pole") {
            out << L"; wydajnoœæ: " << i.GetCapacity() << L" ton." << endl;
        }
        else if (name == L"browar") {
            out << L"; pojemnoœæ: " << i.GetCapacity() << L" ton." << endl;
        }
        else {
            out << L"." << endl;
        }
    }


    printRoadsMatrix(out, this->tab, listVertices , vertices - 2);
    out << endl;
    out << L"æwiartki:" << endl;

    for (const Quarter& q : initialQuarters) {
        out << L"wartoœæ: "<< q.efficiencyMultiplier << L"; punkty graniczne: ";
        for (const auto& point : q.pointTab) {
            out << L"(" << point.first << L"; " << point.second << L"), ";
        }
        out << endl;
    }
    
    //Mapowanie typ?w w?z??w
    unordered_map<int, wstring> tab;
    for ( Node& i : listVertices) {
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
            tab[i.GetId()] = L"skrzy¿owania";
        }
    }

    //wypisanie punkt?w
//    printNodes(out, listVertices);


    out << endl;
    out << L"Rozwi¹zanie:" << endl;
    out << L"Maksymalna iloœæ piwa, któr¹ mo¿na przetransportowaæ: " << maxFlow << L" ton; koszt naprawy: ";
    out << Path::sumUniqueEdgesCost(combined, this->tab) << " srebrnych pensów" << endl;

    // Wypisanie trasy ko?cowej
    int countRoads = 1;
    out << L"Przebieg trasy: " << endl;
    countRoads = 1;
    for (const Path& p : combined) {

        int breweryID = p.getConnectedPoint();
        bool checkBrewery = false;

        out << L"Trasa nr " << countRoads++ << L":" << endl;
        const vector<int>& path = p.getPath();
        for (size_t i = 0; i < path.size() - 1; ++i) {
            int node = path[i];
            wstring stringPole;
            if (i == 0) {
                wstringstream ss;
                ss << L"zawartoœæ pola [" << to_wstring(node) << L"] na pozycji (" << listVertices[node - 1].GetX() << L"; " << listVertices[node - 1].GetY() << L") wynosi " << listVertices[node - 1].GetCapacity() - p.getFlowJeczmien() << L";" << endl;
                stringPole = ss.str();
                listVertices[node - 1].setCapacity(listVertices[node - 1].GetCapacity() - p.getFlowJeczmien());


            }

            out << L"Z " << tab[node] << L" [" << to_wstring(node) << L"] na pozycji (" << listVertices[node - 1].GetX() << L"; " << listVertices[node - 1].GetY() << L") " << (checkBrewery ? p.getFlowPiwo() : p.getFlowJeczmien()) << L" ton " << (checkBrewery ? L"piwa " : L"jêczmienia ");
            if (path[i + 1] == breweryID) {
                checkBrewery = true;
                out << L"do " << tab[path[i + 1]] << L" [" << to_wstring(path[i + 1]) << L"] na pozycji (" << listVertices[path[i + 1] - 1].GetX() << L"; " << listVertices[path[i + 1] - 1].GetY() << L");" << endl;
                if (i == 0) out << stringPole;
                out << L"pojemnoœæ browaru [" << path[i + 1] << L"] na pozycji (" << listVertices[path[i + 1] - 1].GetX() << L"; " << listVertices[path[i + 1] - 1].GetY() << L") wynosi " << (listVertices[path[i + 1] - 1].GetCapacity() - p.getFlowJeczmien()) << L";" << endl;
                listVertices[path[i + 1] - 1].setCapacity(listVertices[path[i + 1] - 1].GetCapacity() - p.getFlowJeczmien());
            }
            else {
                out << L"do " << tab[path[i + 1]] << L" [" << to_wstring(path[i + 1]) << L"] na pozycji (" << listVertices[path[i + 1] - 1].GetX() << L"; " << listVertices[path[i + 1] - 1].GetY() << L");" << endl;
                if (i == 0) out << stringPole;
            }
        }
    }

            out.close();
            //WYPISYWANIE
            wifstream file(L"wynik.txt", ios::binary);
            file.imbue(locale(file.getloc(), new codecvt_utf8<wchar_t>()));
            if (!file.is_open()) {
                wcerr << L"Nie mozna otworzyc pliku wynik.txt" << endl;
            }
            else {
                wstring line;
                wchar_t firstChar = file.peek();
                if (firstChar == 0xFEFF) {
                    file.get(); // zignoruj BOM
                }

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
        //cout << " Droga: z " << s << " do " << t << endl;
        double min = graf[tmp][tmpT].remainingFlow;

        //Wypisuje siezki powieszajace i wyznacza minimum przepustowosci z tej sciezki
        while (tmp != s) {
            //cout << tmpT << " -> " << tmp << " min: " << min << endl;
            tmpT = tmp;
            tmp = f[tmp];
            if (min > graf[tmp][tmpT].remainingFlow) {
                min = graf[tmp][tmpT].remainingFlow;
            }
        }
        //cout << tmpT << " -> " << tmp << " min: " << min << endl;

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

double Matrix::BusackerGowen3(double const maxFlow, int s, int t, vector<Path>& roads, double konwersja, int midLayer,
    bool (Matrix::* shortestPathFunc)(int, int, double&, vector<int>&, const vector<vector<EdgeData>>&)) {

    double result = 0; //przeplyw, zwieksza si? a? nie osiagnie warto?ci F- tj. maxFlow. Podanego jako argument funkcji.
    double summaricResult = 0; //Suma (cost * przeplyw)-jedna sciezka
    double sumCost = 0; // Suma kosztow sciezek
    vector<int> f(vertices); //tablica ojcow
    double cost; //koszt jednej sciezki
    vector<int> path; // lista sciezek do wypisanie
    const double INF = numeric_limits<double>::max();

    // Wykonuje dopoki nie zostal osiagniety przeplyw F(maxFlow)
    // i dopoki istnieje sciezka z s do t (wybiera t? scie?k? kt?ra ma najni?szy koszt)
    while (result < maxFlow && (this->*shortestPathFunc)(s, t, cost, f, tab)) {

        double maxFlowOfPathJeczmien = INF;
        bool conversionChceck = true;
        int connectedPoint;

        int tmpT = t;
        int tmp = f[t];
        double maxFlowOfPathPiwo = tab[tmp][tmpT].remainingFlow;
        cout << " Droga: z " << t << " do " << s << endl;
        cout << tmpT << " -> " << tmp << " f: " << maxFlowOfPathPiwo << endl;
        while (tmp != s) {
            tmpT = tmp;
            tmp = f[tmp];
            if (abs(tmpT - tmp) >= midLayer) {
                conversionChceck = false;
                connectedPoint = tmp;
            }
            if (conversionChceck) {
                maxFlowOfPathPiwo = min(maxFlowOfPathPiwo, tab[tmp][tmpT].remainingFlow);
                cout << tmpT << " -> " << tmp << " f: " << maxFlowOfPathPiwo << endl;
            }
            else {
                maxFlowOfPathJeczmien = min(maxFlowOfPathJeczmien, tab[tmp][tmpT].remainingFlow);
                cout << tmpT << " -> " << tmp << " f: " << maxFlowOfPathJeczmien << endl;
            }
        }

        maxFlowOfPathJeczmien = min(maxFlowOfPathJeczmien, maxFlowOfPathPiwo / konwersja);
        maxFlowOfPathPiwo = min(maxFlowOfPathPiwo, maxFlowOfPathJeczmien * konwersja);

        if (result + maxFlowOfPathPiwo > maxFlow) {
            maxFlowOfPathPiwo = maxFlow - result;
        }

        conversionChceck = true;
        //aktualizuje siec residualna
        tmpT = t;
        tmp = f[t];
        tmpT = tmp;
        tmp = f[tmp];
        while (tmp != s) {
            if (abs(tmpT - tmp) >= midLayer) {
                conversionChceck = false;
            }
            if (conversionChceck) {
                tab[tmp][tmpT].remainingFlow -= maxFlowOfPathPiwo;
                tab[tmpT][tmp].remainingFlow += maxFlowOfPathPiwo;
              //  tab[tmp - midLayer][tmpT - midLayer].remainingFlow -= maxFlowOfPathPiwo;
              //  tab[tmpT - midLayer][tmp - midLayer].remainingFlow += maxFlowOfPathPiwo;
            }
            else {
                tab[tmp][tmpT].remainingFlow -= maxFlowOfPathJeczmien;
                tab[tmpT][tmp].remainingFlow += maxFlowOfPathJeczmien;
                if (tmp != connectedPoint) {
                  //  tab[tmp + midLayer][tmpT + midLayer].remainingFlow -= maxFlowOfPathJeczmien;
                 //   tab[tmpT + midLayer][tmp + midLayer].remainingFlow += maxFlowOfPathJeczmien;
                }
            }
            tab[tmpT][tmp].cost = -tab[tmp][tmpT].cost;
            path.push_back(tmpT);
            tmpT = tmp;
            tmp = f[tmp];
        }
        tab[tmp][tmpT].remainingFlow -= maxFlowOfPathJeczmien;
        tab[tmpT][tmp].remainingFlow += maxFlowOfPathJeczmien;
        tab[tmpT][tmp].cost = -tab[tmp][tmpT].cost;
        path.push_back(tmpT);
        path.push_back(s);
        reverse(path.begin(), path.end());

        roads.push_back(Path(path, maxFlowOfPathPiwo, cost, connectedPoint , maxFlowOfPathJeczmien , maxFlowOfPathPiwo));
        result += maxFlowOfPathPiwo;
        summaricResult += (maxFlowOfPathPiwo * cost);

        //Wypisanie sciezek
        cout << "Sciezka: ";

        for (int val : path) {
            cout << val << " ";
        }

        cout << " Mozna przeslac : " << maxFlowOfPathPiwo << "j piwa .kosztem : " << cost << " Przetworzy? to browar: " << connectedPoint << " .Zatem koszt sciezki : " << (maxFlowOfPathPiwo * cost) << endl;
        path.clear();
        sumCost += cost;
    }

    //wypisanie rezultatow
    cout << "Sumaryczny koszt: " << summaricResult << endl;
    cout << "Suma kosztow drog: " << sumCost << endl;

    //wyjatek
    if (result != maxFlow) {
        cout << "UWAGA! Nie osiagnieto maksymalnego przeplywu. GDZIES JEST BLAD";
    }

    return summaricResult;
}

// ALGORYTM NA MAKSYMALNY PRZEPLYW WYKORZYSTUJACY IDEE SIECI 2 WARSTOWEJ
double Matrix::edmondsKarpClassicWithConversion(int s, int t, double conversion, int midLayer) {
    double maxFlow = 0;
    const double INF = numeric_limits<double>::max();
    vector<int> f(vertices);
    while (bfs(s, tab, t, f))
    {
        double maxFlowOfPathJeczmien = INF;
        bool conversionChceck = true;
        int connectedPoint;

        int tmpT = t;
        int tmp = f[t];
        double maxFlowOfPathPiwo = tab[tmp][tmpT].remainingFlow;
        cout << " Droga: z " << t << " do " << s << endl;
        cout << tmpT << " -> " << tmp << " f: " << maxFlowOfPathPiwo << endl;
        while (tmp != s) {
            tmpT = tmp;
            tmp = f[tmp];
            if (abs(tmpT - tmp) >= midLayer) {
                conversionChceck = false;
                connectedPoint = tmp;
            }
            if (conversionChceck) {
                maxFlowOfPathPiwo = min(maxFlowOfPathPiwo, tab[tmp][tmpT].remainingFlow);
                cout << tmpT << " -> " << tmp << " f: " << maxFlowOfPathPiwo << endl;
            }
            else {
                maxFlowOfPathJeczmien = min(maxFlowOfPathJeczmien, tab[tmp][tmpT].remainingFlow);
                cout << tmpT << " -> " << tmp << " f: " << maxFlowOfPathJeczmien << endl;
            }
        }


        cout << " maks jeczmien " << maxFlowOfPathJeczmien << " maks piwo " << maxFlowOfPathPiwo << endl;

            maxFlowOfPathJeczmien = min(maxFlowOfPathJeczmien, maxFlowOfPathPiwo / conversion);
            maxFlowOfPathPiwo = min(maxFlowOfPathPiwo , maxFlowOfPathJeczmien * conversion);

        cout << " maks jeczmien " << maxFlowOfPathJeczmien << " maks piwo " << maxFlowOfPathPiwo << endl;
        conversionChceck = true;
        //aktualizuje siec residualna
        tmpT = t;
        tmp = f[t];
        tmpT = tmp;
        tmp = f[tmp];
        while (tmp != s) {
            if (abs(tmpT - tmp) >= midLayer) {
                conversionChceck = false;
            }
            if (conversionChceck) {
                tab[tmp][tmpT].remainingFlow -= maxFlowOfPathPiwo;
                tab[tmpT][tmp].remainingFlow += maxFlowOfPathPiwo;
               // tab[tmp - midLayer][tmpT - midLayer].remainingFlow -= maxFlowOfPathPiwo;
               /// tab[tmpT - midLayer][tmp - midLayer].remainingFlow += maxFlowOfPathPiwo;
            }
            else {
                tab[tmp][tmpT].remainingFlow -= maxFlowOfPathJeczmien;
                tab[tmpT][tmp].remainingFlow += maxFlowOfPathJeczmien;
                if (tmp != connectedPoint) {
                   // tab[tmp + midLayer][tmpT + midLayer].remainingFlow -= maxFlowOfPathJeczmien;
                   // tab[tmpT + midLayer][tmp + midLayer].remainingFlow += maxFlowOfPathJeczmien;
                }
            }
            tmpT = tmp;
            tmp = f[tmp];
        }
        tab[tmp][tmpT].remainingFlow -= maxFlowOfPathJeczmien;
        tab[tmpT][tmp].remainingFlow += maxFlowOfPathJeczmien;
 
        maxFlow += maxFlowOfPathPiwo;
    }
    cout << "maksymalny przelyw piwa:" << maxFlow << endl;
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

    cout << "Dla I czesci: ";
    maxFlow = edmondsKarpClassic(s, siecResidualna.tab, midT, f);

    //Resetuje wszystkie powrotne krawedzie ktore nie istnieja w orginalny grafie
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (!(tab[i][j].flow > 0)) {
               siecResidualna.tab[i][j] = EdgeData();
            }
        }
    }

    //Resetuje wszystkie krawedzie od Browar?w do midT 
    //W druga strone zostawiam celowo tj z midT -> browar
    for (Node i : listVertices) {
        if (i.GetType() == Node::NodeType::Brewery) {
            if (siecResidualna.tab[i.GetId()][midT].remainingFlow > 0)
                siecResidualna.tab[i.GetId()][midT].remainingFlow = 0;
               
        }
    }
    cout << "Dla II czesci: ";
    maxFlow1 = edmondsKarpClassic(midT, siecResidualna.tab, t, f);

    return { maxFlow, maxFlow1 };
}

