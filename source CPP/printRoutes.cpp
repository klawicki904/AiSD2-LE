#include "Node.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

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

void printRouteDetails(const vector<vector<int>>& paths, const vector<double>& flows, const vector<Node>& nodes) {
    double total = 0.0;
    cout << fixed << setprecision(1);

    for (double flow : flows) total += flow;

    wcout << L"Rozwi¹zanie:\n";
    wcout << L"Maksymalna iloœæ piwa, któr¹ mo¿na przetransportowaæ: " << total << " ton\n\n";
    wcout << L"Przebieg trasy:\n";

    for (size_t i = 0; i < paths.size(); ++i) {
        wcout << L"Trasa nr " << i + 1 << L":\n";
        const auto& path = paths[i];
        for (size_t j = 0; j < path.size() - 1; ++j) {
            int from = path[j];
            int to = path[j + 1];
            const Node& nFrom = nodes[from];
            const Node& nTo = nodes[to];

            wstring produkt = (nFrom.GetType() == Node::NodeType::Field && nTo.GetType() == Node::NodeType::Brewery) ? L"jêczmienia" : L"piwa";

            wstring typ = nFrom.TypeAsWString();
            wstring z_ze = (typ == L"skrzy¿owanie") ? L"ze " : L"z ";

            wcout << z_ze << L" " << odmien(typ) << L" [" << from << L"] na pozycji (" << nFrom.GetX()
                << L", " << nFrom.GetY() << L") do " << odmien(nTo.TypeAsWString()) << L" [" << to
                << L"] na pozycji (" << nTo.GetX() << L", " << nTo.GetY() << L"): "
                << flows[i] << L" ton " << produkt << L"\n";
        }
        wcout << endl;
    }
}
