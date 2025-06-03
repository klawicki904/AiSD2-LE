#include "Path.h"


Path::Path() : flow(0), cost(0) {}

Path::Path(const vector<int>& path, double flow)
    : path(path), flow(flow), cost(0) {
}

Path::Path(const vector<int>& path, double flow, double cost)
    : path(path), flow(flow), cost(cost) {
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

const vector<int>& Path::getPath() const {
    return path;
}

double Path::getFlow() const {
    return flow;
}

double Path::getCost() const {
    return cost;
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
             std::remove_if(
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

        // Przechodzimy po ka?dej kombinacji i?j, a? znajdziemy takie, które da si? po??czy?
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

                // je?li A.back() != B.front(), to nie da si? ??czy?
                if (endA != startB) continue;

                // Zapisujemy wszystkie potrzebne parametry _PRZED_ erasure:
                double flowA = Apath.getFlow();
                double flowB = Bpath.getFlow();
                double costA = Apath.getCost();
                double costB = Bpath.getCost();
                const vector<int> pathA = A_nodes;  // kopia wektora wierzcho?ków
                const vector<int> pathB = B_nodes;

                // Ile przep?ywu realnie skonsumujemy w po??czeniu:
                double f = min(flowA, flowB);

                // Tworzymy scalon? ?cie?k? (A+B) – tu u?ywamy mergePaths():
                Path merged = mergePaths(Apath, Bpath);
                merged.setFlow(f);
                merged.setCost(costA + costB);

                // Usuwamy „stare” elementy z temp1[i] i temp2[j]:
                temp1.erase(temp1.begin() + i);
                temp2.erase(temp2.begin() + j);

                // Je?li w A zosta?o jeszcze co? (flowA - f > 0), dodajemy „resztkow?” cz???:
                double remFlowA = flowA - f;
                if (remFlowA > 1e-9) {
                    // Koszt przypadaj?cy na jednostk? w A:
                    double costPerUnitA = costA / flowA;
                    double remCostA = costPerUnitA * remFlowA;
                    Path leftoverA(pathA, remFlowA, remCostA);
                    temp1.insert(temp1.begin() + i, leftoverA);
                }
                // Podobnie B:
                double remFlowB = flowB - f;
                if (remFlowB > 1e-9) {
                    double costPerUnitB = costB / flowB;
                    double remCostB = costPerUnitB * remFlowB;
                    Path leftoverB(pathB, remFlowB, remCostB);
                    temp2.insert(temp2.begin() + j, leftoverB);
                }

                // Dodajemy scalon? cz??? do result:
                result.push_back(merged);

                madeMatch = true;
            }
        }
    }
    return result;
}

vector<Path> realPaths;