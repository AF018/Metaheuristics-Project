#pragma once

#include "Heuristics.h"

const int UPPER_BOUND_VALUE = 1e9;

void TabuSearch(Solution& solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);

void TabuSearch_2(Solution solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);