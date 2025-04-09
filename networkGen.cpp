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

bool areSectionsTooClose(vector<int> &sectionTab){
    for(int i=1;i<sectionTab.size();i++)if(sectionTab[i]<=sectionTab[i-1]+1)return true;
    return false;
}

int main() {
    srand(time(nullptr));
    int vertN, sectionN,density;
    cout<<"Ile wierzcholkow powinna posiadac siec?: ";
    cin >> vertN;
    vertN-=2;
    cout<<"Ile zestawow skrzyzowan powinna liczyc siec? (nie liczac pol i miejsc docelowych): ";
    cin>>sectionN;
    cout<<"Jak gesta ma byc powstala siec? (0 - normalna, 1 - gesta, 2 - gesta++): ";
    cin>>density;
    sectionN+=2;
    //sectionN = rand() %3+ (int)log(vertN)+2;
    vector<int> sectionTab(sectionN+1);
    vector<bool> amIConnected(vertN+1);
    vector<vector<pair<int, int>>> vertTab(vertN+1);
    fill(amIConnected.begin(),amIConnected.end(),false);
    amIConnected[0]=true;
    amIConnected[vertN+1]=true;
    sectionTab[0]=0;
    sectionTab[sectionN]=vertN+1;
    {
        do{
        set<int> uniqueSections;
        uniqueSections.clear();
        while(uniqueSections.size()!=sectionN-1)uniqueSections.insert(rand()%(vertN-4)+3);
        int i=1;
        for(auto it=uniqueSections.begin();it!=uniqueSections.end();it++){
            sectionTab[i]=*it;
            i++;
        }
        }while(areSectionsTooClose(sectionTab));
    }
    cout<<endl<<"tablica sectionTab: "<<endl;
    for(int i=0;i<sectionTab.size();i++)cout << sectionTab[i] << " ";
    cout<<endl<<"--------------------------------"<<endl;
    for(int i=1;i<sectionTab[1];i++){
            //vertTab[0].emplace_back(i,INT_MAX);
            vertTab[0].emplace_back(i,99);
            amIConnected[i]=true;
    }
    for(int j=1;j<sectionN;j++)for(int i=sectionTab[j-1];i<sectionTab[j];i++){
        if(i==0)continue;
        int connectionN;
        set<int> uniqueConnections;
        cout<<i<<" "<<j<<endl;
        if(density==2)connectionN=rand()%4+2;
        else if(density==1)connectionN=rand()%2+2;
        else connectionN=1;

        while(uniqueConnections.size()<connectionN)uniqueConnections.insert(rand()%(sectionTab[j+1]-sectionTab[j])+sectionTab[j]);
        for(auto it=uniqueConnections.begin();it!=uniqueConnections.end();it++){
            vertTab[i].emplace_back(*it,rand()%17+5);
            amIConnected[*it]=true;
        }
    }
    for(int i=sectionTab[sectionN-1];i<vertN+1;i++)vertTab[i].emplace_back(vertN+1,99);
    //for(int i=sectionTab[sectionN-1];i<vertN+1;i++)vertTab[i].emplace_back(vertN+1,INT_MAX);
    ///korekty
    for(int j=1;j<sectionN;j++)for(int i=sectionTab[j];i<sectionTab[j+1];i++){
        if(!amIConnected[i]){
        cout<<endl<<"korekta: "<<i<<" "<<j;
        int temp;
        if(j==1)temp=rand()%(sectionTab[1]-1)+1;
        else temp=rand()%(sectionTab[j]-sectionTab[j-1])+sectionTab[j-1];
        vertTab[temp].emplace_back(i,rand()%17+5);
    }
    }
    cout<<endl;
    if(!amIConnected[vertN]){
        int temp=rand()%(sectionTab[sectionN-1]-sectionTab[sectionN-2])+sectionTab[sectionN-2];
        vertTab[temp].emplace_back(vertN,rand()%17+5);
    }


    int finalConnectionN=0;
    for(int i=0;i<=vertN;i++)finalConnectionN+=vertTab[i].size();
    ofstream file("daneZwagami.txt");
    file<<vertN+2<<" "<<finalConnectionN;
    for(int i=0;i<=vertN;i++)for(int j=0;j<vertTab[i].size();j++)file<<endl<<i<<" "<<vertTab[i][j].first<<" "<<vertTab[i][j].second;

    cout<<endl<<vertN+2<<" "<<finalConnectionN<<endl;
    for(int i=0;i<=vertN;i++)for(int j=0;j<vertTab[i].size();j++)cout<<endl<<i<<" "<<vertTab[i][j].first<<" "<<vertTab[i][j].second;

    file.close();
    return 0;
}
