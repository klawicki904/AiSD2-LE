#include "Path.h"
#include "Matrix.h"

Path::Path() : flow(0), cost(0) {}

Path::Path(const vector<int>& path, double flow)
    : path(path), flow(flow), cost(0) {
}

Path::Path(const vector<int>& path, double flow, double cost)
    : path(path), flow(flow), cost(cost) {
}

Path::Path(const vector<int>& path, double flow, double cost, int connectedPoint, double flowJeczmien, double flowPiwo)
    : path(path), flow(flow), cost(cost), connectedPoint(connectedPoint), flowJeczmien(flowJeczmien), flowPiwo(flowPiwo) {
}

void Path::setPath(const vector<int>& newPath) {
    path = newPath;
}

void Path::setFlow(double newFlow) {
    flow = newFlow;
}

void Path::setCost(double newCost) {
    cost = newCost;
}

void Path::setConnectedPoint(int number) {
   connectedPoint = number;
}

const vector<int>& Path::getPath() const {
    return path;
}

double Path::getFlow() const {
    return flow;
}

double Path::getCost() const {
    return cost;
}
int Path::getConnectedPoint() const {
    return connectedPoint;
}

double Path::getFlowJeczmien() const {
    return flowJeczmien;
}
double Path::getFlowPiwo() const {
    return flowPiwo;
}


// Funkcja licz?ca sum? kosztów unikatowych kraw?dzi:
double Path::sumUniqueEdgesCost(const std::vector<Path>& paths, const vector<vector<EdgeData>>& graf) {
    // zbiór odwiedzonych kraw?dzi (u?v)
    std::set<std::pair<int, int>> seen;
    double total = 0.0;

    for (const auto& path : paths) {
        const auto& verts = path.path;
        if (verts.size() < 2) continue;

        for (size_t i = 1; i < verts.size(); ++i) {
            int u = verts[i - 1];
            int v = verts[i];
            auto edge = std::make_pair(u, v);
            // je?li jeszcze nie liczyli?my tej kraw?dzi
            if (seen.insert(edge).second) {
                total += graf[u][v].cost;
            }
        }
    }

    return total;
}

// Funkcja licz?ca sum? przep?ywu:
double Path::sumFlow(const std::vector<Path>& paths) {
    double total = 0.0;

    for (const auto& path : paths) {
        total += path.flow;
    }

    return total;
}

vector<Path> Path::filterRoads(const vector<Path>& raw, int n) {
    int internalCount = n - 2;             // oryginalne w?z?y 1..N-2
    int layerSize = internalCount * 2;  // warstwa II: internalCount+1..2*internalCount

    vector<Path> out;
    out.reserve(raw.size());

    for (auto p : raw) {
        vector<int> newPath;
        newPath.reserve(p.path.size());
        bool removedConnected = false;

        for (auto v : p.path) {
            // Pomijamy ?ród?o (0) i uj?cie (layerSize+1)
            if (v == 0 || v == layerSize + 1) {
                continue;
            }

            // Pomijamy connectedPoint dok?adnie raz
                if (!removedConnected && v == p.connectedPoint) {
                    removedConnected = true;
                    continue;
                }

            // Mapowanie warstwy II na I
            if (v >= internalCount + 1 && v <= layerSize) {
                v -= internalCount;
            }
            // W?z?y warstwy I (1..internalCount) zostaj? bez zmian

            newPath.push_back(v);
        }

        if (!newPath.empty()) {
            vector<int> uniquePath;
            uniquePath.reserve(newPath.size());
            uniquePath.push_back(newPath[0]);
            for (size_t i = 1; i < newPath.size(); ++i) {
                if (newPath[i] != newPath[i - 1]) {
                    uniquePath.push_back(newPath[i]);
                }
            }
            newPath = std::move(uniquePath);
        }

        p.path = std::move(newPath);
        out.push_back(std::move(p));
    }

    return out;
}
    

vector<Path> realPaths;