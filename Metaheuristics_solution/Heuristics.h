#pragma once

#include <numeric>
#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>		/* time */

#include "NeighborGraph.h"

using namespace std;

// Same as NaiveHeuristic, but with random selection among the vertices with highest potential at each iteration
Solution NaiveRandomHeuristic(const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);

// Heuristic creating a solution to the problem, which respects the constraints
Solution NaiveHeuristic(const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);

// Reconstruction of a solution respecting the domination constraint, only used in LocalSearch
void DominationReconstructionHeuristic(Solution& current_solution, const NeighborGraph& captation_graph);

// Reconstruction of a solution respecting the connexity constraint, used in SimulatedAnnealingSearch
void ConnexityReconstructionHeuristic(Solution& current_solution, const NeighborGraph& communication_graph, vector<int>& selected_vertices_vector);