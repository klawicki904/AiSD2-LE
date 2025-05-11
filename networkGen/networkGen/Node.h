#include <vector>
#include <utility>
using namespace std;

#ifndef _NODE_H_
#define _NODE_H_

class Node {
public:
    bool amIConnected;
    bool amIBrewery;
    vector<pair<int, int>> to;
    Node(int vertID, int vertN);
};

#endif // !_NODE_H_