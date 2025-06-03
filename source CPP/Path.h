#ifndef PATH_H
#define PATH_H

#include <vector>
#include <algorithm> 
#include "EdgeData.h"

using namespace std;

class Path {
public:
    Path();
    Path(const vector<int>& path, double flow);
    Path(const vector<int>& path, double flow, double cost); 

    void setPath(const vector<int>& newPath);
    void setFlow(double newFlow);
    void setCost(double newCost);               

    const vector<int>& getPath() const;
    double getFlow() const;
    double getCost() const;   

    //Path mergeWith(const Path& other) const;
    //static vector<Path> mergeVectors(const vector<Path>& first, const vector<Path>& second);

    static vector<Path> filterPaths(const vector<Path>& roads, const vector<vector<EdgeData>>& graph);
    static Path mergePaths(const Path& a, const Path& b);
    static vector<Path> combineRoads(const vector<Path>& raw1, const vector<Path>& raw2, const vector<vector<EdgeData>>& graph);

private:
    vector<int> path;
    double flow;
    double cost = 0.0;
};

extern vector<Path> realPaths;

#endif
