#pragma once

#include "Heuristics.h"

const int UPPER_BOUND_VALUE = 1e9;

void CheckAndAddToPotentialSolutions(const int & potential_vertex_idx, unordered_set<int>& potential_solution_vertices_set,
	const NeighborGraph & communication_graph, unordered_set<int> const & solution_set);

void RemoveVertexFromNeighborPotentialCovering(const int & vertex_idx, const int & vertex_to_delete_idx,
	unordered_map<int, vector<int> >& covering_potential_map, unordered_set<int>& potential_solution_vertices_set);

void SimulatedAnnealingSearch(Solution& solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);

void TabuSearch_2(Solution solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);