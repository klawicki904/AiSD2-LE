#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <set>
#include <fstream>
#include <algorithm>
#include <math.h>
//#include <limits>
#include "Node.h"
using namespace std;

// dziala od 8 wierzcholkow
// dajemy gwarancje dzialania generatora do 40 000 wierzcholkow
// swoja droga pekin posiada okolo 12 000 skrzyzowan
// po co komukolwiek wieksza liczba skrzyzowan ???


// funkcja ta porownuje trzy punkty i stwierdza czy punkt p znajduje sie zgodnie z ruchem wskazowek zegara, czy przeciwnie do nich wzgledem polprostej ab
// zwraca true w przypadku przeciwnym do ruchu wskazowek zegara
static bool Comparator(pair<int, int> a, pair<int, int> b, pair<int, int> p) {
    //cout << "porownuje wektor " << a.first << " " << a.second << " -> " << b.first << " " << b.second << " z punktem " << p.first << " " << p.second;
    int det = (b.first * p.second) + (a.first * b.second) + (a.second * p.first) - (b.first * a.second) - (a.first * p.second) - (b.second * p.first);
    if (det == 0) {
        int distA = (b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second);
        int distB = (p.first - a.first) * (p.first - a.first) + (p.second - a.second) * (p.second - a.second);
        if (distA <= distB) {
            //cout << " dodaje punkt (odleglosc)" << endl;
            return true;
        }
        else {
            //cout << " usuwam punkt (odleglosc)" << endl;
            return false;
        }
    }
    if (det >= 0) {
        //cout << " dodaje punkt (wyznacznik)" << endl;
        return true;
    }
    else {
        //cout << " usuwam punkt (wyznacznik)" << endl;
        return false;
    }
}

int main(int argc, char **argv)
{
    ///---------------wstep---------------
    srand(time(nullptr));
    int vertN, checkpointN, density, fieldN, breweryN, pubN;
    string path;
    // vertN - ilosc wszystkich wierzcholkow w sieci residualnej (moze sie roznic w kodzie ze wzgledu na implementacje i wierzcholki abstrakcyjne)
    // fieldN - ilosc pol
    // breweryN - ilosc browarow
    // pubN - ilosc karczm
    // checkpointN - dlugosc miasta wyznaczona jako najdluzsza ilosc skrzyzowan wymagana do przejscia od startowego do koncowego wierzcholka abstrakcyjnego (tzw. checkpointy)
    // density - wartosc = {0, 1, 2} wyznaczajaca na szanse na "drogi skroty"

   
    //---------------input uzytkownika---------------
    // dane z interfejsu (argumenty linii polecen)
    if (argc == 8) {
        path = argv[1];
        vertN = atoi(argv[2])-2;
        checkpointN = atoi(argv[3]) + 4;
        density = atoi(argv[4]);
        fieldN = atoi(argv[5]);
        breweryN = atoi(argv[6]);
        pubN = atoi(argv[7]);
        if (vertN < 6)return 0;
        if (fieldN <= 0)fieldN = 1;
        if (breweryN <= 0)breweryN = 1;
        if (pubN <= 0)pubN = 1;
    }
    else {
        path = "./generated_map.txt";
        // recznie + obsluga niepoprawnych danych
        // liczba wierzcholkow
        cout << "Ile wierzcholkow powinna posiadac siec?: ";
        cin >> vertN;
        vertN -= 2;     //2 wierzcholki abstrakcyjne
        if (vertN < 6) {
            cout << "Podano za malo wierzcholkow" << endl;
            return 0;
        }

        // liczba pol (wejście uwzglednia wierzcholki abstrakcyjne)
        cout << "Podaj liczbe pol (rekomendowana wartosc == " << round((vertN + 2) * 0.12) << "): ";
        cin >> fieldN;
        if (fieldN <= 0)fieldN = 1;

        // liczba browarow
        cout << "Podaj liczbe browarow (rekomendowana wartosc == " << round((vertN + 2) * 0.24) << "): ";
        cin >> breweryN;
        if (breweryN <= 0)breweryN = 1;

        // liczba karczm
        cout << "Podaj liczbe karczm (rekomendowana wartosc == " << round((vertN + 2) * 0.14) << "): ";
        cin >> pubN;
        if (pubN <= 0)pubN = 1;

        // sprawdzenie, czy w miescie znajduje sie wystarczajaco miejsc na browary
        if (breweryN > vertN - fieldN - pubN) {
            cout << "Podano wiecej browarow niz dostepnych wierzcholkow" << endl;
            return 0;
        }

        // liczba checkpointow (wejscie nie uwzglednia 4 dodatkowych checkpointow niezbednych do stworzenia jakiegokolwiek miasta)
        // wzor wykresu funkcji odpowiadajacy moim zdaniem najbardziej optymalnemu rozlozeniu checkpointow w miescie: y=0.16*vertN^(0.8)
        cout << "Ile zestawow skrzyzowan powinna liczyc siec? (rekomendowana wartosc == " << round(0.16 * pow(vertN, 0.8)) << "): ";
        cin >> checkpointN;
        checkpointN += 4;      //dwa dla wierzcholkow abstrakcyjnych i dwa dla przynajmniej jednego checkpointu

        // liczba checkpointow nie moze byc zbyt duza (to znaczy miasto nie moze byc zbyt dlugie) wowczas powstanie bardzo waskie miasto z mala iloscia drog
        // od poczatku do konca; ponizszy if zapobiega takim danym
        if ((vertN - fieldN - pubN - 5) / 3 + 1 < checkpointN - 4)
        {
            cout << (vertN - fieldN - pubN - 5) / 3 + 1 << " " << checkpointN - 4 << endl;
            cout << "Blad danych wejsciowych (mozliwe problemy):" << endl << "1. Podano zbyt duzo pol i/lub karczm, aby poprawnie wygenerowac siec"
                << endl << "2. Stosunek liczby wierzcholkow do liczby zestawow skrzyzowan jest zbyt maly, aby poprawnie wygenerowac pola i karczmy"
                << endl << "3. Stosunek liczby wierzcholkow do liczby zestawow skrzyzowan jest zbyt maly, aby poprawnie wygenerowac skrzyzowania" << endl;
            return 0;
        }

        // szansa na drogi skroty
        cout << "Jak duza ma byc szansa na drogi-skroty? (0 - normalna, 1 - wieksza, 2 - wieksza++): ";
        cin >> density;
    }
    ///---------------cwiartki---------------
    int quarterPointN;
    vector<set<pair<int, int>>> uniquePointsInSquareSet(4);     // zawiera unikalne punkty dla kazdej z czterech cwiartek
    vector<vector<pair<int, int>>> uniquePointsInSquareTab(4);  // to samo co uniquePointsInSquareSet, ale w wektorze
    int squareArea = sqrt(fieldN) + 10;
    // losowanie punktow do ukladu wspolrzednych dla kazdej z cwiartek
    for (int k = 0; k < 4; k++) {
        quarterPointN = rand() % 15 + 10;
        int counter = 0;
        switch (k) {
        case 0:
            while (uniquePointsInSquareSet[k].size() != quarterPointN)uniquePointsInSquareSet[k].insert(make_pair(rand() % squareArea + 1 , rand() % squareArea + 1));
            break;
        case 1:
            while (uniquePointsInSquareSet[k].size() != quarterPointN)uniquePointsInSquareSet[k].insert(make_pair(-1 * (rand() % squareArea + 1), rand() % squareArea + 1));
            break;
        case 2:
            while (uniquePointsInSquareSet[k].size() != quarterPointN)uniquePointsInSquareSet[k].insert(make_pair(-1 * (rand() % squareArea + 1), -1 * (rand() % squareArea + 1)));
            break;
        case 3:
            while (uniquePointsInSquareSet[k].size() != quarterPointN)uniquePointsInSquareSet[k].insert(make_pair(rand() % squareArea + 1, -1 * (rand() % squareArea + 1)));
            break;
        }
    }
    // przepisanie ze zbioru do wektora
    for (int k = 0; k < 4; k++)for (auto it = uniquePointsInSquareSet[k].begin(); it != uniquePointsInSquareSet[k].end(); it++)uniquePointsInSquareTab[k].emplace_back(it->first, it->second);

    // dla kazdej cwiartki wyznaczam najnizszy punkt (wspolrzedna y), a nastepnie sortuje cala tablice za pomoca funkcji Comparator
    for (int k = 0; k < 4; k++) {
        int lowestYid = 0;
        int lowestYValue = uniquePointsInSquareTab[k][0].second;
        for (int i = 1; i < uniquePointsInSquareTab[k].size(); i++)if (uniquePointsInSquareTab[k][i].second < lowestYValue) {
            lowestYid = i;
            lowestYValue = uniquePointsInSquareTab[k][i].second;
        }
        swap(uniquePointsInSquareTab[k][0], uniquePointsInSquareTab[k][lowestYid]);
        sort(uniquePointsInSquareTab[k].begin(), uniquePointsInSquareTab[k].end(), [&](pair<int, int> a, pair<int, int> b) {return Comparator(uniquePointsInSquareTab[k][0], a, b); });
    }
    // uzywam wektora quarters jako kolejki, poniewaz potrzebuje dostepu do ostatnich dwoch elementow
    // wyznaczam na podstawie poprzednich losowo wybranych punktow wynikowy wielokat wypukly
    // za pomoca algorytmu otoczki wypuklej z wykorzystaniem funkcji Comparator
    vector<vector<pair<int, int>>> quarters(4);
    for (int k = 0; k < 4; k++) {
        quarters[k].push_back(uniquePointsInSquareTab[k][0]);
        quarters[k].push_back(uniquePointsInSquareTab[k][1]);

        for (int i = 2; i < uniquePointsInSquareTab[k].size(); i++) {
            if (Comparator(quarters[k][quarters[k].size() - 2], quarters[k][quarters[k].size() - 1], uniquePointsInSquareTab[k][i]))quarters[k].push_back(uniquePointsInSquareTab[k][i]);
            else {
                quarters[k].pop_back();
                while (!Comparator(quarters[k][quarters[k].size() - 2], quarters[k][quarters[k].size() - 1], uniquePointsInSquareTab[k][i]))quarters[k].pop_back();
                quarters[k].push_back(uniquePointsInSquareTab[k][i]);
            }
        }
        quarters[k].push_back(uniquePointsInSquareTab[k][0]);
    }

    //---------------przygotowanie struktur danych---------------
    vector<int> checkpointTab;      // wektor zawierajacy indeksy checkpointow

    vector<Node *> vertTab(vertN + 1);          // wektor zawierajacy wskazniki na klasy Node reprezentujace dany punkt grafu
    for (int i = 0; i <= vertN; i++)vertTab[i] = new Node(i, vertN);
    vertTab[0]->coordinates = make_pair(-squareArea-5, 0);

    ///---------------losowanie przedzialow---------------
    // byla to najtrudniejsza czesc projektu do zaimplementowania
    // modifier ustala ze kazdy checkpoint jest w odleglosci przynajmniej <wartosc modifier> wierzcholkow od innych checkpointow
    // odleglosc ta automatycznie sie skaluje, wiec jesli jest za malo miejsca to odleglosci sa mniejsze
    // priorytezujemy duze odleglosci w celu zbalansowania wizualnego miasta (zobaczymy czy ten model sie nadaje w praktyce) (spoiler: powinien sie nadawac)

    int modifier = checkpointN;

    // na podstawie wolnego miejsca obliczamy pesymistyczny przypadek wylosowania checkpointow, tak aby dobrac odpowiedni modifier
    // uwaga duzo matematyki
    for (int i = modifier-1; i >=0; i--) {
        modifier--;
        if ((vertN - fieldN - pubN - 3 * i - 2) < 1)continue;
        if ((vertN - fieldN - pubN - 3 * i - 2) / (2 * i + 1) + 1 >= checkpointN - 4)break;
    }
    if ((vertN - fieldN - pubN - 5) / 3 <= 0)modifier = 1;


    cout << endl << "Modifier: " << modifier <<" Minimum w sekcji: "<<modifier+1<< endl;
    cout << "Maksymalna dozwolona ilosc checkpointow: " << (vertN - fieldN - pubN - 5) / 3 + 1 << endl;
    cout << "Rozmiar wektora kandydatow: " << vertN - fieldN - pubN - 2*modifier-1  << endl;
    cout << "Ilosc checkpointow: " << checkpointN - 4 << endl;
    if (modifier == 0) {
        cout << "Jesli to widzisz to znaczy ze nie powinienes tego widziec, sprobuj wiecej wierzcholkow lub mniej zestawow nwm..." << endl;
        return 0;
    }

    // wektor indeksow sposrod ktorych beda losowane indeksy checkpointow; w trakcie wyznaczania sa z niego usuwane sasiednie wartosci kandydatow na checkpointy
    vector<int> checkpointCandidateTab(vertN - fieldN - pubN - 2*modifier-1);
    for (int i = 0; i < checkpointCandidateTab.size(); i++)checkpointCandidateTab[i] = i + fieldN + modifier + 2;
    cout << "Zakres losowanych indeksow skrzyzowan: " << checkpointCandidateTab[0] << " - " << checkpointCandidateTab[checkpointCandidateTab.size() - 1] << endl;

    // wstepne wartosci indeksow checkpointow (pewniaczki)
    checkpointTab.push_back(0);                     // startowy wierzcholek abstrakcyjny
    checkpointTab.push_back(vertN + 1);             // koncowy wierzcholek abstrakcyjny
    checkpointTab.push_back(fieldN + 1);            // indeks wskazujacy koniec pol + 1
    checkpointTab.push_back(vertN + 1 - pubN);      // indeks wskazujacy poczatek karczm

    // dodawanie kolejnych losowo wybranych checkpointow
    while (checkpointTab.size() != checkpointN)
    {
        int section = rand() % checkpointCandidateTab.size();
        checkpointTab.push_back(checkpointCandidateTab[section]);
        //cout << "Wybrano: " << checkpointCandidateTab[section] << " "<<endl;
        int startIndex = max(0, section - modifier);
        int endIndex = min(static_cast<int>(checkpointCandidateTab.size()), section + modifier + 1);
        vector<int> values(endIndex - startIndex);      // wartosci sasiednych indeksow do porownania, czy nalezy je wyrzucic z wektora, czy nie
        for (int i = 0; i < values.size(); i++)values[i] = checkpointCandidateTab[section] - (section - startIndex) + i;
        for (int i = startIndex, k = 0; i < endIndex; i++, k++) {
            //cout << checkpointCandidateTab[i] << " " << values[k] << " ";
            if (checkpointCandidateTab[i] == values[k]) {
                //cout << "Usuwam: " << checkpointCandidateTab[i] << endl;
                checkpointCandidateTab.erase(checkpointCandidateTab.begin() + i);
                i--;
                endIndex--;
            }
        }
    }
    sort(checkpointTab.begin(), checkpointTab.end(), [](int a, int b){return a < b;});

    cout << endl << "tablica checkpointTab: " << endl;
    for (int i = 0; i < checkpointTab.size(); i++)cout << checkpointTab[i] << " ";
    cout << endl << "--------------------------------" << endl << "Podsumowanie:" << endl;

    ///---------------losowanie wspolrzednych pol---------------
    set<pair<int,int>> fieldCoordinatesSet;    // zbior unikalnych wspolrzednych punktow na plaszczyznie cwiartek
    int quarterIndex;                           // indeks cwiartki = {0, 1, 2, 3}
    bool isPointValid;                          // true jesli punkt nalezy do cwiartki, false jesli nie
    int fieldCoordinatesSetSize = 0;            // ostatni zapisany rozmiar zbioru; sprawdza czy nie bylo juz identycznego punktu w zbiorze
    pair<int,int> point;                       // punkt o losowych wspolrzednych w danej cwiartce
    // kazde kolejne pole trafia do kolejnej cwiartki
    for (int i = 1; i <= fieldN; i++) {
        quarterIndex = (i-1) % 4;
        isPointValid = true;
        switch (quarterIndex) {
        case 0:
            point=make_pair(rand() % squareArea + 1, rand() % squareArea + 1);
            break;
        case 1:
            point = make_pair(-1 * (rand() % squareArea + 1), rand() % squareArea + 1);
            break;
        case 2:
            point=make_pair(-1 * (rand() % squareArea +1), -1 * (rand() % squareArea +1));
            break;
        case 3:
            point=make_pair(rand() % squareArea + 1, -1 * (rand() % squareArea +1));
            break;
        }

        // sprawdzenie czy pole w ogole znajduje sie w cwiartce
        for (int j = 0; j < quarters[quarterIndex].size()-1; j++) {
            if (!Comparator(quarters[quarterIndex][j], quarters[quarterIndex][j+1], point))isPointValid = false;
        }

        // jesli tak, to jest dodawane, jesli nie to losowanie odbywa sie jeszcze raz
        fieldCoordinatesSetSize = fieldCoordinatesSet.size();
        fieldCoordinatesSet.insert(point);
        if (isPointValid && fieldCoordinatesSetSize!=fieldCoordinatesSet.size())vertTab[i]->coordinates = point;
        else i--;
    }

    ///---------------wyznaczanie wspolrzednych pozostalych punktow---------------
    int x=0;
    int y=0;

    // pozostale punkty sa ustawiane na wschod od cwiartek rzedami zgodnie z wylosowanymi checkpointami
    for (int j = 1; j < checkpointN - 1; j++)for (int i = checkpointTab[j]; i < checkpointTab[j + 1]; i++) {
        x = squareArea+4*j;
        int y;
        int nodesInSection = checkpointTab[j + 1] - checkpointTab[j];
        //cout << i <<" "<<j << endl;
        y = nodesInSection - 1 - ((i - checkpointTab[j]) * 2);
        vertTab[i]->coordinates = make_pair(x, y);
    }
    x += 4;
    y = 0;

    ///---------------losowanie browarow---------------
    // browary sa losowane sposrod wierzcholkow ktorym nie przypisano zadnej roli (skrzyzowan)
    set<int> uniqueBreweries;
    uniqueBreweries.clear();
    while (uniqueBreweries.size() != breweryN)uniqueBreweries.insert(rand() % (checkpointTab[checkpointN-2] -1 - checkpointTab[1]) + checkpointTab[1]);
    for (auto it = uniqueBreweries.begin(); it != uniqueBreweries.end(); it++)vertTab[*it]->amIBrewery = true;

    ///---------------wierzcholek startowy---------------   !!! DO ZMIANY - WARTOSCI POWINNY ODPOWIADAC PRODUKCJI POL !!!
    for (int i = 1; i < checkpointTab[1]; i++)vertTab[0]->to.emplace_back(i, 99);

    cout << "Liczba pol: " << checkpointTab[1] - 1 << endl;

    ///---------------drogi-skroty---------------
    int connectionN;            // oznacza ilosc polaczen wychodzacych 
    int connectionInfo = 0;     // zlicza ilosc "drog-skrotow" do podsumowania
    int connectionChance;       // okresla szanse od ktorej zalezy ile z danego wierzcholka wyjdzie drog skrotow

    for (int j = 1; j < checkpointN - 2; j++)for (int i = checkpointTab[j - 1]; i < checkpointTab[j]; i++)
    {
        connectionN = 0;
        connectionChance = rand() % 10;
        if (i == 0)continue;
        switch (density)
        {
        case 0:
            if (connectionChance > 6)connectionN = 1;
            break;
        case 1:
            if (connectionChance > 4)connectionN = 1;
            break;
        case 2:
            if (connectionChance > 6 && pubN>1)connectionN = 2;
            else if (connectionChance < 3)connectionN = 1;
            break;
        }
        if (connectionN==0)continue;

        connectionInfo += connectionN;

        set<int> uniqueConnections;
        uniqueConnections.clear();

        // losujemy unikalne indeksy "drog-skrotow" do skutku
        while (uniqueConnections.size() != connectionN)uniqueConnections.insert(rand() % (checkpointTab[checkpointN-1] - checkpointTab[j + 1]) + checkpointTab[j + 1]);
        for (auto it = uniqueConnections.begin(); it != uniqueConnections.end(); it++) {
            //cout << "zdalne polaczenie: " << i << " -> " << *it << " w sekcji " << j << " w liczbie : " << connectionN << endl;
            vertTab[i]->to.emplace_back(*it, rand() % 17 + 5);
            vertTab[*it]->amIConnected = true;
        }
    }

    cout << "Liczba drog-skrotow: " << connectionInfo << endl;
    connectionInfo = 0;

    ///---------------tworzenie polaczen miedzy sasiednimi przedzialami---------------      !!! DO ZMIANY - MOZLIWOSC ZMIANY GESTOSCI POLACZEN !!!
    for (int j = 1; j < checkpointN-1; j++)for (int i = checkpointTab[j - 1]; i < checkpointTab[j]; i++)
    {
        if (i == 0)continue;
        //connectionN = 0;
        
        /*     !!! FUNKCJA WYLACZONA DO ODWOLANIA PRZEPRASZAMY ZA PROBLEMY TECHNICZNE !!!
        switch (density)
        {
        case 0:
            connectionN = 1;
            break;
        case 1:
            if (rand() % 4 == 0)connectionN = 2;
            else connectionN = 1;
            break;
        case 2:
            if (checkpointTab[j + 1] - checkpointTab[j] <= 4)connectionN = rand() % 1 + 1;
            else connectionN = rand() % 2 + 1;
            break;
        }
        */
        
        int connectionChance = rand() % 10;
        if (connectionChance > 6)connectionN = 2;
        else connectionN = 1;
        //cout<<"liczba kolejnych polaczen: "<<connectionN<<endl;
        
        ///zobaczymy czy sie przyjmie
        if (connectionN > checkpointTab[j + 1] - checkpointTab[j])connectionN = checkpointTab[j + 1] - checkpointTab[j];
        connectionN -= vertTab[i]->to.size();
        if (connectionN < 1)continue;
        connectionInfo += connectionN;

        set<int> uniqueConnections;
        while (uniqueConnections.size() < connectionN)uniqueConnections.insert(rand() % (checkpointTab[j + 1] - checkpointTab[j]) + checkpointTab[j]);
        for (auto it = uniqueConnections.begin(); it != uniqueConnections.end(); it++) {
            vertTab[i]->to.emplace_back(*it, rand() % 17 + 5);
            vertTab[*it]->amIConnected = true;
            //cout << "sasiednie polaczenie: " << i << " -> "<<*it<<" w sekcji " << j << " dotychczasowe polaczenia: " << vertTab[i]->to.size() << " " << endl;
        }
    }
    cout << "Liczba drog laczacych sasiednie skrzyzowania: " << connectionInfo << endl;
    connectionInfo = 0;

    ///---------------koncowy wierzcholek---------------        !!! DO ZMIANY - WARTOSC KONCOWYCH POLACZEN DO USTALENIA !!!
    for (int i = checkpointTab[checkpointN - 2]; i < vertN + 1; i++)vertTab[i]->to.emplace_back(vertN + 1, 99);
    //for(int i=checkpointTab[checkpointN-1];i<vertN+1;i++)vertTab[i].emplace_back(vertN+1,INT_MAX);

    ///---------------korekty---------------
    // korekty polaczen zapewniaja, ze do kazdego wierzcholka nie bedacego startowym wchodzi przynajmniej jedna droga
    for (int j = 1; j < checkpointN-1; j++)for (int i = checkpointTab[j]; i < checkpointTab[j + 1]; i++)
    {
        if (!vertTab[i]->amIConnected)
        {
            // postanowilem wyrownac ilosc polaczen-korekt, poniewaz siec jest wtedy "ladniejsza"
            // z drugiej jednak strony psuje to losowosc - w koncu polaczenie nie jest losowo wybierane tylko ustalone      !!! DO ROZKMINIENIA - LOSOWANIE SPOSROD ZBIORU CZY STARY SYSTEM LOSOWANIA
            // aktualnie jesli do wierzcholka nie wchodzi zadna droga, to wybierany wierzcholek-poprzednik jest pierwszy z brzegu pod wzgledem najmniejszej ilosc wychodzacych polaczen
            int minimumConnectionNValue;
            int minimumConnectionNIndex;
            minimumConnectionNIndex = checkpointTab[j - 1];
            if (minimumConnectionNIndex == 0)minimumConnectionNIndex++;
            minimumConnectionNValue = vertTab[minimumConnectionNIndex]->to.size();

            for (int k = minimumConnectionNIndex; k < checkpointTab[j]; k++) {
                if (vertTab[k]->to.size() < minimumConnectionNValue)
                {
                    minimumConnectionNValue = vertTab[k]->to.size();
                    minimumConnectionNIndex = k;
                }
            }

            // glowna czesc korekty
            //cout<<"korekta: "<<minimumConnectionNIndex<<" -> "<<i << " w sekcji " << j << endl;
            connectionInfo++;
            vertTab[minimumConnectionNIndex]->to.emplace_back(i, rand() % 17 + 5);
            vertTab[i]->amIConnected = true;
        }
    }

    cout << "Liczba korekt drog: " << connectionInfo << endl;
    cout << "Liczba karczm: " << checkpointTab[checkpointN-1] - checkpointTab[checkpointN - 2] << endl;
    int globalConnectionN = 0;
    for (int i = 0; i <= vertN; i++)globalConnectionN += vertTab[i]->to.size();

    ///---------------wypisanie wyniku---------------
    // wersja visualgo
    //ofstream file("daneZwagami.txt");
    //file << vertN + 2 << " " << globalConnectionN;
    //for (int i = 0; i <= vertN; i++)for (int j = 0; j < vertTab[i]->to.size(); j++)file << endl << i << " " << vertTab[i]->to[j].first << " " << vertTab[i]->to[j].second;
    //file.close();

    // wersja oficjalna zgodna z szablonem danych wejsciowych ZAWIERA DROGI DWUKIERUNKOWE
    float roadCost;
    ofstream experimentalFile(path);
    experimentalFile << "KONWERSJA" << endl << static_cast <float>(rand()) / static_cast <float>(RAND_MAX) << endl<<"PUNKTY"<<endl;
    experimentalFile <<"0 "<< vertTab[0]->coordinates.first << " " << vertTab[0]->coordinates.second << " brak" << endl;
    for (int i = 1; i < checkpointTab[1]; i++)experimentalFile << i << " "<<vertTab[i]->coordinates.first<<" "<<vertTab[i]->coordinates.second<<" pole" << endl;
    for (int i = checkpointTab[1]; i < checkpointTab[checkpointN - 2]; i++) {
        if (vertTab[i]->amIBrewery)experimentalFile << i <<" "<< vertTab[i]->coordinates.first << " " << vertTab[i]->coordinates.second << " browar " << 
            static_cast <float>(rand()) / static_cast <float>(RAND_MAX) + 20 + rand() % 12 <<endl;
        else experimentalFile << i << " "<<vertTab[i]->coordinates.first << " " << vertTab[i]->coordinates.second << " brak" << endl;
    }
    for (int i = checkpointTab[checkpointN - 2]; i < checkpointTab[checkpointN - 1]; i++)experimentalFile << i<<" "<<vertTab[i]->coordinates.first<<" "<<vertTab[i]->coordinates.second<<" karczma" << endl;
    experimentalFile << vertN+1<<" "<< x << " " << y << " brak" << endl << "DROGI";
    //experimentalFile << "DROGI";
    for (int i = 0; i <= vertN; i++)for (int j = 0; j < vertTab[i]->to.size(); j++) {
        if (rand() % 4 == 0)roadCost = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) + rand() % 70;
        else roadCost = 0;
        experimentalFile << endl << i << " " << vertTab[i]->to[j].first << " " << vertTab[i]->to[j].second << " " << roadCost;
        //if (i != 0 && i != vertN)experimentalFile << endl << vertTab[i]->to[j].first << " " << i << " " << vertTab[i]->to[j].second << " " << roadCost;
    }
    experimentalFile << endl << "CWIARTKI";
    for (int k = 0; k < 4; k++) {
        experimentalFile << endl<<rand() % 9900 + 100;
        for (int i = 0; i < quarters[k].size()-1; i++)experimentalFile<<" "<< quarters[k][i].first << " " << quarters[k][i].second;
        
    }
    experimentalFile.close();

    // debug log na standardowe wyjscie
    cout << endl <<"Liczba wszystkich wierzcholkow: "<<vertN + 2 << endl << "Liczba wszystkich polaczen: "<<globalConnectionN << endl;
    for (int i = 0; i <= vertN; i++)for (int j = 0; j < vertTab[i]->to.size(); j++)cout << endl << i << " " << vertTab[i]->to[j].first << " " << vertTab[i]->to[j].second;
    
    // otwieram katalog z wynikami
    //system("explorer .");

    for (int i = 0; i <= vertN; i++)delete vertTab[i];

    return 0;
}
