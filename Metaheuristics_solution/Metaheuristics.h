#pragma once

#include "Heuristics.h"

const int UPPER_BOUND_VALUE = 1e9;

void read_parameter_file(string const & parameter_path, string& file_path, string& heuristic_result_path, string& SA_result_path,
	double& captation_radius, double& communication_radius,
	int& reconstruction_threshold, int& SA_iteration_number,
	double& init_temperature, double& decreasing_coef, double& final_temperature);

void CheckAndAddToPotentialSolutions(const int & potential_vertex_idx, unordered_set<int>& potential_solution_vertices_set,
	const NeighborGraph & communication_graph, unordered_set<int> const & solution_set);

void RemoveVertexFromNeighborPotentialCovering(const int & vertex_idx, const int & vertex_to_delete_idx,
	unordered_map<int, vector<int> >& covering_potential_map, unordered_set<int>& potential_solution_vertices_set);

void SimulatedAnnealingSearch(Solution& solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph,
	const double& init_temperature, const double& final_temperature, const double& decreasing_coef, const int& reconstruction_threshold);

void LocalSearch(Solution solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);