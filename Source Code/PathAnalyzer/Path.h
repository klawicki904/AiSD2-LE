#pragma once
#ifndef PATH_H
#define PATH_H


#include <vector>
#include <algorithm> 
#include "EdgeData.h"
#include <set>
#include <utility> // dla std::pair

using namespace std;

class Matrix;

class Path {
public:
    Path();
    Path(const vector<int>& path, double flow);
    Path(const vector<int>& path, double flow, double cost); 
    Path(const vector<int>& path, double flow, double cost , int connectedPoint, double flowJeczmien , double flowPiwo);

    void setPath(const vector<int>& newPath);
    void setFlow(double newFlow);
    void setCost(double newCost);
    void setConnectedPoint(int number);

    const vector<int>& getPath() const;
    double getFlow() const;
    double getCost() const;   
    int getConnectedPoint() const;
    double getFlowJeczmien() const;
    double getFlowPiwo() const;


    //Path mergeWith(const Path& other) const;
    //static vector<Path> mergeVectors(const vector<Path>& first, const vector<Path>& second);

    static vector<Path> filterPaths(const vector<Path>& roads, const vector<vector<EdgeData>>& graph);
    static Path mergePaths(const Path& a, const Path& b);
    static vector<Path> combineRoads(const vector<Path>& raw1, const vector<Path>& raw2, const vector<vector<EdgeData>>& graph);
    static vector<Path> filterRoads(const vector<Path>& raw, int n);

    static double sumUniqueEdgesCost(const std::vector<Path>& paths, const vector<vector<EdgeData>>& graf);
    static double sumFlow(const std::vector<Path>& paths);

private:
    vector<int> path;
    double flow;
    double cost = 0.0;
    int connectedPoint;
    double flowJeczmien;
    double flowPiwo;
};

extern vector<Path> realPaths;

#endif
