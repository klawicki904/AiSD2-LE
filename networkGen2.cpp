#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <set>
#include <fstream>
#include <math.h>
using namespace std;

int main() {
    srand(time(nullptr));
    int vertN, sectionN;
    cin >> vertN;

    sectionN = rand() %3+ (int)log(vertN)+2;

    vector<int> sectionTab(sectionN+2);
    vector<bool> amIConnected(vertN+1);
    vector<vector<pair<int, int>>> vertTab(vertN+1);
    fill(amIConnected.begin(),amIConnected.end(),false);
    amIConnected[0]=true;
    amIConnected[vertN]=true;

    int sectionTemp = 0;
    sectionTab[0]=0;
    sectionTab[sectionN]=vertN;
    for (int i = 1; i < sectionN; i++) {
        sectionTab[i] = rand() % 3 + (vertN / sectionN) + sectionTemp - 2;
        sectionTemp = sectionTab[i];
    }

    for(int i=0;i<sectionN;i++)cout << sectionTab[i] << " ";
    for(int i=1;i<sectionTab[1];i++){
            vertTab[0].emplace_back(i,rand()%17+5);
            amIConnected[i]=true;
    }
    for(int j=1;j<sectionN;j++)for(int i=sectionTab[j-1];i<sectionTab[j];i++){
        if(i==0)continue;
        int connectionN;
        set<int> uniqueConnections;
        connectionN=(sectionTab[j+1]-sectionTab[j])/3+rand()%1+1;
        while(uniqueConnections.size()<connectionN-1)uniqueConnections.insert(rand()%(sectionTab[j+1]-sectionTab[j]+1)+sectionTab[j]);
        for(auto it=uniqueConnections.begin();it!=uniqueConnections.end();it++){
            vertTab[i].emplace_back(*it,rand()%17+5);
            amIConnected[*it]=true;
        }
    }
    for(int i=sectionTab[sectionN-1];i<vertN+1;i++)vertTab[i].emplace_back(vertN+1,rand()%17+5);
    int finalConnectionN=0;
    int finalNodeN=0;
    ofstream file("daneZwagami.txt");
    for(int i=0;i<vertN+1;i++)for(int j=0;j<vertTab[i].size();j++)if(amIConnected[i])finalConnectionN++;
    file<<vertN+2<<" "<<finalConnectionN;
    for(int i=0;i<vertN+1;i++)for(int j=0;j<vertTab[i].size();j++)if(amIConnected[i])file<<endl<<i<<" "<<vertTab[i][j].first<<" "<<vertTab[i][j].second;

    cout<<endl<<vertN+2<<" "<<finalConnectionN<<endl;
    for(int i=0;i<vertN+1;i++)for(int j=0;j<vertTab[i].size();j++)cout<<endl<<i<<" "<<vertTab[i][j].first<<" "<<vertTab[i][j].second;

    file.close();
    return 0;
}
