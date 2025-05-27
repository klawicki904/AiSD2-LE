#include "Quarter.h"
using namespace std;

Quarter::Quarter() {
	efficiencyMultiplier = 1.0;
}

bool Quarter::comparator(pair<double, double> a, pair<double, double> b, pair<double, double> p) {
    //cout << "porownuje wektor " << a.first << " " << a.second << " -> " << b.first << " " << b.second << " z punktem " << p.first << " " << p.second;
    double det = (b.first * p.second) + (a.first * b.second) + (a.second * p.first) - (b.first * a.second) - (a.first * p.second) - (b.second * p.first);
    if (det == 0) {
        double distA = (b.first - a.first) * (b.first - a.first) + (b.second - a.second) * (b.second - a.second);
        double distB = (p.first - a.first) * (p.first - a.first) + (p.second - a.second) * (p.second - a.second);
        if (distA <= distB)return true;
        return false;
    }
    if (det >= 0)return true;
    return false;
}

void Quarter::sort() {
        int lowestYid = 0;
        double lowestYValue = pointTab[0].second;
        for (int i = 1; i < pointTab.size(); i++)if (pointTab[i].second < lowestYValue) {
            lowestYid = i;
            lowestYValue = pointTab[i].second;
        }
        swap(pointTab[0], pointTab[lowestYid]);
        std::sort(pointTab.begin(), pointTab.end(), [&](pair<double, double> a, pair<double, double> b) {return comparator(pointTab[0], a, b); });
}