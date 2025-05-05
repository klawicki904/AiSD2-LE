#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <set>
#include <fstream>
#include <math.h>
#include <algorithm>
//#include <limits>
using namespace std;


// NOTKA:
// dziala od 8 wierzcholkow
// przykladowe dane dla testow:
// 40 9 ? ?
// 20 4 ? ?
// gestosc dziala lepiej dla wiekszej liczby wierzcholkow (czasami zdarza sie ze przy gestosci na poziomie 1 jest tyle samo krawedzi co na poziomie 2 przy 20 wierzcholkach)
// dziala rowniez dla wiekszych danych np. 10 000+ ale nie bylem w stanie zweryfikowac wynikow ze strona visualgo (strona przy 150-200 wierzcholkach przestaje dzialac)
// wspolczynnik liczby wierzcholkow do liczby zestawow skrzyzowan nie moze byc mniejszy niz 2.6
/// nie przesadzaj z liczba zestawow skrzyzowan !!! (dwa sa dodane defaultowo wiec mozesz wpisac 0, wowczas powstana polaczenia bezposrednie)


bool areSectionsTooClose(int section, set<int>&uniqueSections, int vsRatio)
{
    int modifier=3;
    if(vsRatio<6)modifier=2;
    if(vsRatio<4)modifier=1;
    for(auto it=uniqueSections.begin(); it!=uniqueSections.end(); it++)if(abs(*it-section)<=modifier)return true;
    return false;
}

int main()
{
    ///wstep
    srand(time(nullptr));
    int vertN, sectionN,density;
    bool remoteConnectionsHigherChance=false;
    cout<<"Ile wierzcholkow powinna posiadac siec?: ";
    cin >> vertN;
    vertN-=2;
    cout<<"Ile zestawow skrzyzowan powinna liczyc siec? (nie liczac pol i miejsc docelowych): ";
    cin>>sectionN;
    sectionN+=2;
    if((float)vertN/sectionN<2.6){
        cout<<"Stosunek liczby wierzcholkow do liczby skrzyzowan jest zbyt maly, aby poprawnie wygenerowac siec"<<endl;
        return 0;
    }
    cout<<"Jak gesta ma byc powstala siec? (0 - normalna, 1 - gesta, 2 - gesta++): ";
    cin>>density;
    cout<<"Jak roznorodne maja byc polaczenia? (0 - normalna, 1 - bardziej roznorodne polaczenia): ";
    cin>>remoteConnectionsHigherChance;
    //sectionN = rand() %3+ (int)log(vertN)+2;
    vector<int> sectionTab(sectionN+1);
    vector<bool> amIConnected(vertN+1);
    vector<vector<pair<int, int>>> vertTab(vertN+1);
    fill(amIConnected.begin(),amIConnected.end(),false);
    amIConnected[0]=true;
    amIConnected[vertN+1]=true;
    sectionTab[0]=0;
    sectionTab[sectionN]=vertN+1;

    ///losowanie przedzialow
    set<int> uniqueSections;
    while(uniqueSections.size()!=sectionN-1)
    {
        int section=rand()%(vertN-4)+3;
        if(!areSectionsTooClose(section,uniqueSections,vertN/sectionN))
        {
            uniqueSections.insert(section);
        }
    }
    {
        int i=1;
        for(auto it=uniqueSections.begin(); it!=uniqueSections.end(); it++)
        {
            sectionTab[i]=*it;
            i++;
        }
    }
    cout<<endl<<"tablica sectionTab: "<<endl;
    for(int i=0; i<sectionTab.size(); i++)cout << sectionTab[i] << " ";
    cout<<endl<<"--------------------------------"<<endl;
    ///wierzcholek startowy
    for(int i=1; i<sectionTab[1]; i++)
    {
        //vertTab[0].emplace_back(i,INT_MAX);
        vertTab[0].emplace_back(i,99);
        amIConnected[i]=true;
    }
    int connectionN;

    for(int j=1; j<sectionN-1; j++)for(int i=sectionTab[j-1]; i<sectionTab[j]; i++)
        {
            bool areAnyConnections=true;
            if(i==0)continue;
            switch(density)
            {
            case 0:
                if(rand()%4==0 && !remoteConnectionsHigherChance)connectionN=1;
                else if(rand()%3==0 && remoteConnectionsHigherChance)connectionN=1;
                else areAnyConnections=false;
                break;
            case 1:
                if(rand()%3==0 && !remoteConnectionsHigherChance)connectionN=1;
                else if(rand()%2==0 && remoteConnectionsHigherChance)connectionN=1;
                else areAnyConnections=false;
                break;
            case 2:
                if(rand()%3==0)connectionN=rand()%2+1;
                else areAnyConnections=false;
                break;
            }
            if(!areAnyConnections)continue;
            //cout<<"zdalne polaczenie: "<<i<<" "<<j<<endl;
            set<int> uniqueConnections;
            uniqueConnections.clear();
            while(uniqueConnections.size()!=connectionN)uniqueConnections.insert(rand()%(sectionTab[sectionN]-sectionTab[j+1])+sectionTab[j+1]);
            for(auto it=uniqueConnections.begin(); it!=uniqueConnections.end(); it++)
            {
                vertTab[i].emplace_back(*it,rand()%17+5);
                amIConnected[*it]=true;
            }
        }

    ///tworzenie polaczen miedzy sasiednimi przedzialami
    for(int j=1; j<sectionN; j++)for(int i=sectionTab[j-1]; i<sectionTab[j]; i++)
        {
            if(i==0)continue;
            connectionN=0;
            set<int> uniqueConnections;
            //cout<<"sasiednie polaczenie: "<<i<<" "<<j<<" dotychczasowe polaczenia: "<<vertTab[i].size()<<" ";
            switch(density)
            {
            case 0:
                connectionN=1;
                break;
            case 1:
                if(rand()%3==0)connectionN=2;
                else connectionN=1;
                break;
            case 2:
                if(sectionTab[j+1]-sectionTab[j]<=4)connectionN=rand()%2+1;
                else connectionN=rand()%3+1;
                break;
            }

            //cout<<"liczba kolejnych polaczen: "<<connectionN<<endl;
            ///zobaczymy czy sie przyjmie
            connectionN-=vertTab[i].size();
            if(connectionN<1)continue;

            while(uniqueConnections.size()<connectionN)uniqueConnections.insert(rand()%(sectionTab[j+1]-sectionTab[j])+sectionTab[j]);
            for(auto it=uniqueConnections.begin(); it!=uniqueConnections.end(); it++)
            {
                vertTab[i].emplace_back(*it,rand()%17+5);
                amIConnected[*it]=true;
            }
        }
    ///koncowy wierzcholek
    for(int i=sectionTab[sectionN-1]; i<vertN+1; i++)vertTab[i].emplace_back(vertN+1,99);
    //for(int i=sectionTab[sectionN-1];i<vertN+1;i++)vertTab[i].emplace_back(vertN+1,INT_MAX);
    ///korekty
    for(int j=1; j<sectionN; j++)for(int i=sectionTab[j]; i<sectionTab[j+1]; i++)
        {
            if(!amIConnected[i])
            {
                //cout<<"korekta: "<<i<<" "<<j<<endl;
                int temp;
                if(j==1)temp=rand()%(sectionTab[1]-1)+1;
                else temp=rand()%(sectionTab[j]-sectionTab[j-1])+sectionTab[j-1];
                vertTab[temp].emplace_back(i,rand()%17+5);
            }
        }
    if(!amIConnected[vertN])
    {
        int temp=rand()%(sectionTab[sectionN-1]-sectionTab[sectionN-2])+sectionTab[sectionN-2];
        vertTab[temp].emplace_back(vertN,rand()%17+5);
    }

    cout<<"--------------------------------"<<endl;

    ///wypisanie wyniku
    int finalConnectionN=0;
    for(int i=0; i<=vertN; i++)finalConnectionN+=vertTab[i].size();
    ofstream file("daneZwagami.txt");
    file<<vertN+2<<" "<<finalConnectionN;
    for(int i=0; i<=vertN; i++)for(int j=0; j<vertTab[i].size(); j++)file<<endl<<i<<" "<<vertTab[i][j].first<<" "<<vertTab[i][j].second;

    cout<<endl<<vertN+2<<" "<<finalConnectionN<<endl;
    for(int i=0; i<=vertN; i++)for(int j=0; j<vertTab[i].size(); j++)cout<<endl<<i<<" "<<vertTab[i][j].first<<" "<<vertTab[i][j].second;

    file.close();
    return 0;
}