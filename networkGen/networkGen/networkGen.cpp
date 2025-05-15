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


// NOTKA:
// dziala od 10 wierzcholkow
// dziala rowniez dla wiekszych danych np. 10 000+ ale nie bylem w stanie zweryfikowac wynikow ze strona visualgo (strona przy 150-200 wierzcholkach przestaje dzialac)
// wspolczynnik liczby wierzcholkow do liczby zestawow skrzyzowan nie moze byc mniejszy niz 2.6

/// nie przesadzaj z liczba zestawow skrzyzowan !!! (cztery sa dodane defaultowo wiec mozesz wpisac 0)

#define MINIMUM_CORRECT_RATIO 2.6   //znalazlem metoda prob i bledow najmniejszy stosunek liczby wierzcholkow do zestawow skrzyzowan ktory wygeneruje "ladna" mape skrzyzowan
                                    //nie chcemy skrzyzowan do ktorych przykladowo wchodzi milion drog a wychodzi tylko jedna

#define MINIMUM_SECTION_POSITION_N 3    //minimalna dopuszczalna ilosc skrzyzowan w zestawie

static bool areSectionsTooClose(int section, set<int>& uniqueSections, int vsRatio)
{
    int modifier = 3;
    if (vsRatio < 6)modifier = 2;
    if (vsRatio < 4)modifier = 1;
    for (auto it = uniqueSections.begin(); it != uniqueSections.end(); it++)if (abs(*it - section) <= modifier)return true;
    return false;
}

bool comparator(pair<int, int> s, pair<int, int> a, pair<int, int> b) {
    //cout << "porownuje wektor " << s.first << " " << s.second << " -> " << a.first << " " << a.second << " z punktem " << b.first << " " << b.second;
    int det = (a.first * b.second) + (s.first * a.second) + (s.second * b.first) - (a.first * s.second) - (s.first * b.second) - (a.second * b.first);
    if (det == 0) {
        int distA = (a.first - s.first) * (a.first - s.first) + (a.second - s.second) * (a.second - s.second);
        int distB = (b.first - s.first) * (b.first - s.first) + (b.second - s.second) * (b.second - s.second);
        if (distA < distB) {
            //cout << " dodaje punkt (odleglosc)" << endl;
            return true;
        }
        else {
            //cout << " usuwam punkt (odleglosc)" << endl;
            return false;
        }
    }
    if (det > 0) {
        //cout << " dodaje punkt (wyznacznik)" << endl;
        return true;
    }
    else {
        //cout << " usuwam punkt (wyznacznik)" << endl;
        return false;
    }
}

int main()
{
    ///wstep
    srand(time(nullptr));
    int vertN, sectionN, density, fieldN, breweryN, pubN;

    ///cwiartki (musze posprzatac te spaghetti)
    int quarterPointN;
    vector<set<pair<int, int>>> incompleteQuartersSet(4);
    vector<vector<pair<int, int>>> incompleteQuartersTab(4);
    for (int k = 0; k < 4; k++) {
        quarterPointN = rand() % 18 + 6;
        switch (k) {
        case 0:
            while (incompleteQuartersSet[k].size() != quarterPointN)incompleteQuartersSet[k].insert(make_pair(rand() % 100+1, rand() % 100+1));
            break;
        case 1:
            while (incompleteQuartersSet[k].size() != quarterPointN)incompleteQuartersSet[k].insert(make_pair(-1 * (rand() % 100+1), rand() % 100+1));
            break;
        case 2:
            while (incompleteQuartersSet[k].size() != quarterPointN)incompleteQuartersSet[k].insert(make_pair(-1 * (rand() % 100+1), -1 * (rand() % 100+1)));
            break;
        case 3:
            while (incompleteQuartersSet[k].size() != quarterPointN)incompleteQuartersSet[k].insert(make_pair(rand() % 100+1, -1 * (rand() % 100+1)));
            break;
        }
    }
    for (int k = 0; k < 4; k++)for (auto it = incompleteQuartersSet[k].begin(); it != incompleteQuartersSet[k].end(); it++)incompleteQuartersTab[k].emplace_back(it->first,it->second);
    for (int k = 0; k < 4; k++) {
        int lowestYid = 0, lowestY = incompleteQuartersTab[k][0].second;
        for (int i = 1; i < incompleteQuartersTab[k].size(); i++)if (incompleteQuartersTab[k][i].second < lowestY) {
            lowestYid = i;
            lowestY = incompleteQuartersTab[k][i].second;
        }
        swap(incompleteQuartersTab[k][0], incompleteQuartersTab[k][lowestYid]);
        //cout << endl << "pierwszy punkt: " << incompleteQuartersTab[k][0].first << " " << incompleteQuartersTab[k][0].second << endl;
        sort(incompleteQuartersTab[k].begin(), incompleteQuartersTab[k].end(), [&](pair<int, int> a, pair<int, int> b) {return comparator(incompleteQuartersTab[k][0], a, b); });
        //cout << endl << "posortowane: " << endl<<endl;
    }
    
    for (int i = 0; i < incompleteQuartersTab.size(); i++) {
        //for (int j = 0; j < incompleteQuartersTab[i].size(); j++)cout << incompleteQuartersTab[i][j].first << " " << incompleteQuartersTab[i][j].second << endl;
        //cout << endl;
    }
    //-----------------------------
    vector<vector<pair<int, int>>> quarters(4);
    for (int k = 0; k < 4; k++) {
        quarters[k].push_back(incompleteQuartersTab[k][0]);
        quarters[k].push_back(incompleteQuartersTab[k][1]);

        for (int i = 2; i < incompleteQuartersTab[k].size(); i++) {
            if (comparator(quarters[k][quarters[k].size() - 2], quarters[k][quarters[k].size() - 1], incompleteQuartersTab[k][i]))quarters[k].push_back(incompleteQuartersTab[k][i]);
            else {
                quarters[k].pop_back();
                while (!comparator(quarters[k][quarters[k].size() - 2], quarters[k][quarters[k].size() - 1], incompleteQuartersTab[k][i]))quarters[k].pop_back();
                quarters[k].push_back(incompleteQuartersTab[k][i]);
            }
        }
    } 

    //---------------input uzytkownika---------------
    cout << "Ile wierzcholkow powinna posiadac siec?: ";     //pola + skrzyzowania + browary + karczmy + punkty startu i konca
    cin >> vertN;
    vertN -= 2;     //2 wierzcholki abstrakcyjne
    if (vertN / 5 - 1 < 0)cout << "Ile zestawow skrzyzowan powinna liczyc siec? (rekomendowana wartosc == 0): ";     //przedzialow zawierajacych same skrzyzowania
    else cout << "Ile zestawow skrzyzowan powinna liczyc siec? (rekomendowana wartosc == " << vertN / 5 - 1 << "): ";     //przedzialow zawierajacych same skrzyzowania
    cin >> sectionN;
    sectionN += 4;
    if ((float)vertN / (sectionN-2) < MINIMUM_CORRECT_RATIO)
    {
        cout << "Stosunek liczby wierzcholkow do liczby skrzyzowan jest zbyt maly, aby poprawnie wygenerowac siec" << endl;
        return 0;
    }
    cout << "Podaj liczbe pol: ";
    cin >> fieldN;
    if (fieldN <= 0)fieldN=1;
    cout << "Podaj liczbe browarow: ";
    cin >> breweryN;
    if (breweryN <= 0)breweryN=1;
    cout << "Podaj liczbe karczm: ";
    cin >> pubN;
    if (pubN <= 0)pubN=1;
    if ((float)(vertN - fieldN - pubN) / (sectionN - 2) < MINIMUM_SECTION_POSITION_N)
    {
        cout <<"Blad danych wejsciowych (mozliwe problemy):"<<endl<<"1. Podano zbyt duzo browarow i/lub karczm, aby poprawnie wygenerowac siec"
            << endl <<"2. Stosunek liczby wierzcholkow do liczby skrzyzowan jest zbyt maly, aby poprawnie wygenerowac pola i karczmy";
        return 0;
    }
    cout << "Jak duza ma byc szansa na drogi-skroty? (0 - normalna, 1 - wieksza, 2 - wieksza++): ";       //szansa na wylosowanie drogi skrotu
                                                                                                      //im wieksza szansa tym tez wiecej wierzcholkow
    cin >> density;

    //---------------przygotowanie struktur danych---------------
    vector<int> sectionTab(sectionN);
    vector<Node *> vertTab(vertN + 1);
    for (int i = 0; i <= vertN; i++)vertTab[i] = new Node(i, vertN);
    sectionTab[0] = 0;
    sectionTab[sectionN-1] = vertN + 1;

    ///---------------losowanie przedzialow---------------
    set<int> uniqueSections;
    uniqueSections.insert(fieldN+1);
    uniqueSections.insert(vertN + 1 - pubN);
    while (uniqueSections.size() < sectionN - 2)
    {
        int section = rand() % (vertN-pubN-fieldN) + fieldN+1;
        if (!areSectionsTooClose(section, uniqueSections, vertN / sectionN))
        {
            uniqueSections.insert(section);
        }
    }
    {
        int i = 1;
        for (auto it = uniqueSections.begin(); it != uniqueSections.end(); it++)
        {
            sectionTab[i] = *it;
            i++;
        }
    }
    cout << endl << "tablica sectionTab: " << endl;
    for (int i = 0; i < sectionTab.size(); i++)cout << sectionTab[i] << " ";
    cout << endl << "--------------------------------" << endl;

    ///---------------losowanie browarow---------------
    set<int> uniqueBreweries;
    uniqueBreweries.clear();
    while (uniqueBreweries.size() != breweryN)uniqueBreweries.insert(rand() % (sectionTab[sectionN-2] -1 - sectionTab[1]) + sectionTab[1]);
    for (auto it = uniqueBreweries.begin(); it != uniqueBreweries.end(); it++)vertTab[*it]->amIBrewery = true;

    ///---------------wierzcholek startowy---------------
    for (int i = 1; i < sectionTab[1]; i++)vertTab[0]->to.emplace_back(i, 99);

    cout << "Liczba pol: " << sectionTab[1] - 1 << endl;

    ///---------------drogi-skroty---------------
    int localConnectionN;
    int localConnectionInfo = 0;
    int localConnectionNChance;
    for (int j = 1; j < sectionN - 2; j++)for (int i = sectionTab[j - 1]; i < sectionTab[j]; i++)
    {
        localConnectionN = 0;
        localConnectionNChance = rand() % 10;
        if (i == 0)continue;
        switch (density)
        {
        case 0:
            if (localConnectionNChance > 6)localConnectionN = 1;
            break;
        case 1:
            if (localConnectionNChance > 3)localConnectionN = 1;
            break;
        case 2:
            if (localConnectionNChance > 5 && pubN>1)localConnectionN = 2;
            else if (localConnectionNChance < 2)localConnectionN = 1;
            break;
        }
        if (localConnectionN==0)continue;
        
        localConnectionInfo += localConnectionN;

        set<int> uniqueConnections;
        uniqueConnections.clear();
        while (uniqueConnections.size() != localConnectionN)uniqueConnections.insert(rand() % (sectionTab[sectionN-1] - sectionTab[j + 1]) + sectionTab[j + 1]);
        for (auto it = uniqueConnections.begin(); it != uniqueConnections.end(); it++) {
            //cout << "zdalne polaczenie: " << i << " -> " << *it << " w sekcji " << j << " w liczbie : " << localConnectionN << endl;
            vertTab[i]->to.emplace_back(*it, rand() % 17 + 5);
            vertTab[*it]->amIConnected = true;
        }
    }
    cout << "Liczba drog-skrotow: " << localConnectionInfo << endl;
    localConnectionInfo = 0;

    ///---------------tworzenie polaczen miedzy sasiednimi przedzialami---------------
    for (int j = 1; j < sectionN-1; j++)for (int i = sectionTab[j - 1]; i < sectionTab[j]; i++)
    {
        if (i == 0)continue;
        //localConnectionN = 0;
        
        /*     !!! FUNKCJA WYLACZONA DO ODWOLANIA PRZEPRASZAMY ZA PROBLEMY TECHNICZNE !!!
        switch (density)
        {
        case 0:
            localConnectionN = 1;
            break;
        case 1:
            if (rand() % 4 == 0)localConnectionN = 2;
            else localConnectionN = 1;
            break;
        case 2:
            if (sectionTab[j + 1] - sectionTab[j] <= 4)localConnectionN = rand() % 1 + 1;
            else localConnectionN = rand() % 2 + 1;
            break;
        }
        */
        localConnectionN = 1;
        //cout<<"liczba kolejnych polaczen: "<<localConnectionN<<endl;

        ///zobaczymy czy sie przyjmie
        localConnectionN -= vertTab[i]->to.size();
        if (localConnectionN < 1)continue;
        localConnectionInfo += localConnectionN;
        
        set<int> uniqueConnections;
        while (uniqueConnections.size() < localConnectionN)uniqueConnections.insert(rand() % (sectionTab[j + 1] - sectionTab[j]) + sectionTab[j]);
        for (auto it = uniqueConnections.begin(); it != uniqueConnections.end(); it++) {
            vertTab[i]->to.emplace_back(*it, rand() % 17 + 5);
            vertTab[*it]->amIConnected = true;
            //cout << "sasiednie polaczenie: " << i << " -> "<<*it<<" w sekcji " << j << " dotychczasowe polaczenia: " << vertTab[i]->to.size() << " " << endl;
        }
    }
    cout << "Liczba drog laczacych sasiednie skrzyzowania: " << localConnectionInfo << endl;
    localConnectionInfo = 0;

    ///---------------koncowy wierzcholek---------------
    for (int i = sectionTab[sectionN - 2]; i < vertN + 1; i++)vertTab[i]->to.emplace_back(vertN + 1, 99);
    //for(int i=sectionTab[sectionN-1];i<vertN+1;i++)vertTab[i].emplace_back(vertN+1,INT_MAX);

    ///---------------korekty---------------
    for (int j = 1; j < sectionN-1; j++)for (int i = sectionTab[j]; i < sectionTab[j + 1]; i++)
    {
        if (!vertTab[i]->amIConnected)
        {
            int minimumConnectionNValue;
            int minimumConnectionNIndex;

            if (j == 1) minimumConnectionNIndex = sectionTab[j - 1] + 1;
            else minimumConnectionNIndex = sectionTab[j - 1];
               
            minimumConnectionNValue = vertTab[minimumConnectionNIndex]->to.size();

            for (int k = sectionTab[j - 1] + 1; k < sectionTab[j] - 1; k++)if (vertTab[k]->to.size() < minimumConnectionNValue)
            {
                minimumConnectionNValue = vertTab[k]->to.size();
                minimumConnectionNIndex = k;
            }
            //cout<<"korekta: "<<minimumConnectionNIndex<<" -> "<<i << " w sekcji " << j << endl;
            localConnectionInfo++;
            vertTab[minimumConnectionNIndex]->to.emplace_back(i, rand() % 17 + 5);
            vertTab[i]->amIConnected = true;
        }
    }
    cout << "Liczba korekt drog: " << localConnectionInfo << endl;
    cout << "Liczba karczm: " << sectionTab[sectionN-1] - sectionTab[sectionN - 2] << endl;
    cout << "--------------------------------" << endl;
    int connectionN = 0;
    for (int i = 0; i <= vertN; i++)connectionN += vertTab[i]->to.size();

    ///---------------wypisanie wyniku---------------
    ofstream file("daneZwagami.txt");
    file << vertN + 2 << " " << connectionN;
    for (int i = 0; i <= vertN; i++)for (int j = 0; j < vertTab[i]->to.size(); j++)file << endl << i << " " << vertTab[i]->to[j].first << " " << vertTab[i]->to[j].second;
    file.close();

    ofstream experimentalFile("daneEksperymentalne.txt");
    experimentalFile << "KONWERSJA" << endl << static_cast <float>(rand()) / static_cast <float>(RAND_MAX) << endl<<"PUNKTY"<<endl;
    experimentalFile << "0 x y punkt_abstrakcyjny" << endl;
    for (int i = 1; i < sectionTab[1]; i++)experimentalFile << i << " x y pole" << endl;
    for (int i = sectionTab[1]; i < sectionTab[sectionN - 2]; i++) {
        if (vertTab[i]->amIBrewery)experimentalFile << i << " x y browar" << endl;
        else experimentalFile << i << " x y brak" << endl;
    }
    for (int i = sectionTab[sectionN - 2]; i < sectionTab[sectionN - 1]; i++)experimentalFile << i<<" x y karczma" << endl;
    experimentalFile << vertN+1<<" x y punkt_abstrakcyjny" << endl<<"DROGI";
    for (int i = 0; i <= vertN; i++)for (int j = 0; j < vertTab[i]->to.size(); j++) {
        float roadCost = static_cast <float>(rand()) / static_cast <float>(RAND_MAX) + rand() % 100;
        experimentalFile << endl << i << " " << vertTab[i]->to[j].first << " " << vertTab[i]->to[j].second << " " << roadCost;
        if (i != 0 && i != vertN)experimentalFile << endl << vertTab[i]->to[j].first << " " << i << " " << vertTab[i]->to[j].second << " " << roadCost;
    }
    experimentalFile << endl << "CWIARTKI" << endl;
    for (int k = 0; k < 4; k++) {
        experimentalFile << rand() % 9900 + 100;
        for (int i = 0; i < quarters[k].size(); i++)experimentalFile << " " << quarters[k][i].first << " " << quarters[k][i].second;
        experimentalFile << endl;
    }
    experimentalFile.close();

    cout << endl <<"Liczba wszystkich wierzcholkow: "<<vertN + 2 << endl << "Liczba wszystkich polaczen: "<<connectionN << endl;
    for (int i = 0; i <= vertN; i++)for (int j = 0; j < vertTab[i]->to.size(); j++)cout << endl << i << " " << vertTab[i]->to[j].first << " " << vertTab[i]->to[j].second;

    //zwolnienie pamieci
    for (int i = 0; i <= vertN; i++)delete vertTab[i];
    
    return 0;
}
