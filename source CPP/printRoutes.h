#pragma once

#ifndef PRINTROUTES_H
#define PRINTROUTES_H

#include <vector>

#include <fstream> //

#include "Node.h"

void printRouteDetails(std::wofstream& out, const std::vector<std::vector<int>>& paths,
    const std::vector<double>& flows,
    const std::vector<Node>& nodes);

#endif