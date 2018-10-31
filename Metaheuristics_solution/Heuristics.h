#pragma once

#include <numeric>
#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>		/* time */

#include "NeighborGraph.h"

using namespace std;

Solution NaiveRandomHeuristic(const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);

Solution NaiveHeuristic(const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);

void DominationReconstructionHeuristic(Solution& current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);

void ConnexityReconstructionHeuristic(Solution& current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);