#include "EdgeData.h"
#include "Node.h"
#include <iostream>
#include <string>
#include <vector>

#include <locale>
#include <windows.h>

using namespace std;

inline wstring odmien(const wstring& typ) {
    if (typ == L"pole") return L"pola";
    if (typ == L"browar") return L"browaru";
    if (typ == L"karczma") return L"karczmy";
    if (typ == L"skrzyzowanie") return L"skrzy¿owania";

    return typ; // domyœlna nazwa bez odmiany
}

void printNodes(const vector<Node>& nodes) {
    wcout << L"Punkty:\n";
    for (const auto& node : nodes) {
        wstring typ = node.TypeAsWString();
        wcout << L"Punkt " << node.GetId() << L": typ: " << typ << L"; pozycja: x = " << node.GetX() << L", y = " << node.GetY();

        if (typ == L"pole") {
            wcout << L"; wydajnosc: " << node.GetCapacity() << L" ton.\n";
        }
        if (typ == L"browar") {
            wcout << L"; pojemnosc: " << node.GetCapacity() << L" ton.\n";
        }
        else {
            wcout << L".\n";
        }
    }
    wcout << endl;
}

void printRoads(const vector<vector<EdgeData>>& nList, const vector<Node>& nodes) {
    wcout << L"Drogi:\n";
    int roadCounter = 1;

    for (int u = 0; u < nList.size(); ++u) {
        for (const auto& edge : nList[u]) {
            const Node& from = nodes[u];
            const Node& to = nodes[edge.v];
            wstring typ = from.TypeAsWString();
            wstring z_ze = (typ == L"skrzy¿owanie") ? L"ze" : L"z";

            wcout << L"Droga " << roadCounter++ << L": " << z_ze << L" " << odmien(typ) << L" [" << from.GetId()
                << L"] na pozycji (" << from.GetX() << L", " << from.GetY() << L") do "
                << odmien(to.TypeAsWString()) << " [" << to.GetId() << L"] na pozycji ("
                << to.GetX() << L", " << to.GetY() << L"): przepustowoœæ: "
                << edge.remainingFlow << L" ton; koszt naprawy: "
                << edge.cost << L" srebrnych pensów.\n";
        }
    }
    wcout << endl;
}
