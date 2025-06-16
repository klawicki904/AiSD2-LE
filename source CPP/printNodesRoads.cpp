#include <iostream>

#include "printNodesRoads.h"
#include "EdgeData.h"
#include "Node.h"


#include <string>
#include <vector>

#include <fstream>

using namespace std;

inline wstring odmien(const wstring& typ) {
    if (typ == L"pole") return L"pola";
    if (typ == L"browar") return L"browaru";
    if (typ == L"karczma") return L"karczmy";
    if (typ == L"skrzyzowanie") return L"skrzy¿owania";

    return typ; // domyœlna nazwa bez odmiany
}

void printNodes(std::wofstream& out, const vector<Node>& nodes) {
    out << L"Punkty:\n";
    for (const auto& node : nodes) {
        wstring typ = node.TypeAsWString();
        out << L"Punkt " << node.GetId() << L": typ: " << typ << L"; pozycja: x = " << node.GetX() << L", y = " << node.GetY();

        if (typ == L"pole") {                                                                                                       //////////////////////////////////
            out << L"; wydajnoœæ: " << node.GetCapacity() << L" ton.\n";
        }
        if (typ == L"browar") {
            out << L"; pojemnoœæ: " << node.GetCapacity() << L" ton.\n";
        }
        else {
            out << L".\n";
        }
    }
    out << endl;
}

void printRoads(std::wofstream& out, const vector<vector<EdgeData>>& nList, const vector<Node>& nodes) {
    out << L"Drogi:\n";
    int roadCounter = 1;

    for (int u = 0; u < nList.size(); ++u) {
        for (const auto& edge : nList[u]) {
            const Node& from = nodes[u];
            const Node& to = nodes[edge.v];
            wstring typ = from.TypeAsWString();
            wstring z_ze = (typ == L"skrzy¿owanie") ? L"ze" : L"z";

            out << L"Droga " << roadCounter++ << L": " << z_ze << L" " << odmien(typ) << L" [" << from.GetId()
                << L"] na pozycji (" << from.GetX() << L"; " << from.GetY() << L") do "
                << odmien(to.TypeAsWString()) << " [" << to.GetId() << L"] na pozycji ("
                << to.GetX() << L"; " << to.GetY() << L"): przepustowoœæ: "
                << edge.remainingFlow << L" ton; koszt naprawy: "
                << edge.cost << L" srebrnych pensów.\n";
        }
    }
    out << endl;
}

void printRoadsMatrix(std::wofstream& out, const vector<vector<EdgeData>>& nList, const vector<Node>& nodes, int size) {
    out << endl;
    out << L"Drogi:\n";
    int roadCounter = 1;
    for (int u = 1; u < size + 1 ; ++u) {
        for (int v = 1; v < size + 1; ++v) {
            if (nList[u][v].flow > 0) {
                const Node& from = nodes[u -1];
                const Node& to = nodes[v - 1];
                wstring typ = from.TypeAsWString();
                wstring z_ze = (typ == L"skrzy¿owanie") ? L"ze" : L"z";

                out << L"Droga " << roadCounter++ << L": " << z_ze << L" " << odmien(typ) << L" [" << from.GetId()
                    << L"] na pozycji (" << from.GetX() << L"; " << from.GetY() << L") do "
                    << odmien(to.TypeAsWString()) << " [" << to.GetId() << L"] na pozycji ("
                    << to.GetX() << L"; " << to.GetY() << L"): przepustowoœæ: "
                    << nList[u][v].flow << L" ton; koszt naprawy: "
                    << nList[u][v].cost << L" srebrnych pensów.\n";
            }
        }
    }
}
