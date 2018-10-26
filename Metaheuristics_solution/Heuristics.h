#pragma once

#include <numeric>
#include <unordered_set>

#include "NeighborGraph.h"

Solution NaiveHeuristic(NeighborGraph captation_graph, const NeighborGraph& communication_graph);
