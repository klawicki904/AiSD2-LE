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


 vector<Path> Path::filterPaths(
    const vector<Path>& roads,
    const vector<vector<EdgeData>>& graph
) {
     vector<Path> result;
     result.reserve(roads.size());

     for (const Path& p : roads) {

         //  Pobierz wektor wierzcho?ków
         vector<int> cleaned = p.getPath();

         //  Usuwa wszystkie wierzcho?ki równe 0, midT lub vertices-2
         cleaned.erase(
             remove_if(
                 cleaned.begin(),
                 cleaned.end(),
                 [&](int v) {
                     return v == 0
                         || v == graph.size() - 1
                         || v == (graph.size() - 2);
                 }
             ),
             cleaned.end()
         );
         
         if (!cleaned.empty()) {
             Path newPath = p;         // skopiuj oryginalny obiekt
             newPath.setPath(cleaned); // podmie? tylko list? wierzcho?ków
             result.push_back(move(newPath));
         }
     }
     return result;
 }


Path Path::mergePaths(const Path& a, const Path& b) {
    const vector<int>& A = a.getPath();
    const vector<int>& B = b.getPath();

    if (A.empty()) return b;
    if (B.empty()) return a;

    // Je?li ko?ce si? nie zgadzaj?, nie ??czymy – zwracamy a (lub b)
    if (A.back() != B.front()) {
        return a;
    }

    vector<int> merged = A;
    // Doklejamy tylko B[1..], ?eby nie powtarza? punktu A.back() == B.front()
    merged.insert(merged.end(), B.begin() + 1, B.end());

    double mergedFlow = min(a.getFlow(), b.getFlow());
    double mergedCost = a.getCost() + b.getCost();
    return Path(merged, mergedFlow, mergedCost);
}


vector<Path> Path::combineRoads(
    const vector<Path>& raw1,
    const vector<Path>& raw2,
    const vector<vector<EdgeData>>& graph
) {
    vector<Path> temp1 = filterPaths(raw1, graph);
    vector<Path> temp2 = filterPaths(raw2, graph);

    vector<Path> result;
    result.reserve(temp1.size() + temp2.size());

    bool madeMatch = true;
    while (madeMatch) {
        madeMatch = false;

        for (size_t i = 0; i < temp1.size() && !madeMatch; ++i) {
            const Path& Apath = temp1[i];
            const vector<int>& A_nodes = Apath.getPath();
            if (A_nodes.empty()) continue;
            int endA = A_nodes.back();

            for (size_t j = 0; j < temp2.size() && !madeMatch; ++j) {
                const Path& Bpath = temp2[j];
                const vector<int>& B_nodes = Bpath.getPath();
                if (B_nodes.empty()) continue;
                int startB = B_nodes.front();

                if (endA != startB) continue;

                double flowA = Apath.getFlow();
                double flowB = Bpath.getFlow();
                double costA = Apath.getCost();
                double costB = Bpath.getCost();
                const vector<int> pathA = A_nodes;
                const vector<int> pathB = B_nodes;

                double f = min(flowA, flowB);

                Path merged = mergePaths(Apath, Bpath);
                // Ustawiamy punkt ??cz?cy ?cie?ki:
                merged.setConnectedPoint(endA);
                merged.setFlow(f);
                merged.setCost(costA + costB);

                temp1.erase(temp1.begin() + i);
                temp2.erase(temp2.begin() + j);

                double remFlowA = flowA - f;
                if (remFlowA > 1e-9) {

                    Path leftoverA(pathA, remFlowA, remFlowA);
                    temp1.insert(temp1.begin() + i, leftoverA);
                }
                double remFlowB = flowB - f;
                if (remFlowB > 1e-9) {
                    Path leftoverB(pathB, remFlowB, remFlowB);
                    temp2.insert(temp2.begin() + j, leftoverB);
                }

                result.push_back(merged);
                madeMatch = true;
            }
        }
    }
    return result;
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

        p.path = std::move(newPath);
        out.push_back(std::move(p));
    }

    return out;
}
    

vector<Path> realPaths;