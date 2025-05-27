#ifndef QUARTER_H
#define QUARTER_H

#include<iostream>
#include<vector>
#include<utility>
#include <algorithm>
using namespace std;
class Quarter
{
public:
	vector<pair<double, double>> pointTab;
	double efficiencyMultiplier;
	Quarter();
	void sort();
	static bool comparator(pair<double, double> a, pair<double, double> b, pair<double, double> p);
};
#endif