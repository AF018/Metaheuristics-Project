#pragma once

#include <numeric>

#include "NeighborGraph.h"

using namespace std;

Solution NaiveHeuristic(const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);

void ReconstructionHeuristic(Solution& current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);