#pragma once

#ifndef PRINTNODESROADS_H
#define PRINTNODESROADS_H

#include <vector>

#include <fstream> //

#include "Node.h"
#include "EdgeData.h"

void printNodes(std::wofstream& out, const std::vector<Node>& nodes);
void printRoads(std::wofstream& out, const vector<vector<EdgeData>>& nList, const vector<Node>& nodes);
std::wstring odmien(const std::wstring& typ); //wspolna funkcja dla funkcji printowych

#endif