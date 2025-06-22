#include <vector>
#include <utility>
using namespace std;

#ifndef _NODE_H_
#define _NODE_H_

class Node {
public:
    bool amIConnected;
    bool amIBrewery;
    vector<pair<int, int>> to;      //  .first = id wierzcholka do ktorego idzie polaczenie; .second = przepustowosc tego polaczenia
    pair<int,int> coordinates;
    Node(int vertID, int vertN);
};

#endif // !_NODE_H_