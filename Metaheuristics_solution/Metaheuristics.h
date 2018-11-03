#pragma once

#include "Heuristics.h"

const int UPPER_BOUND_VALUE = 1e9;

// Reads a parameter file and modifies the arguments with these values
void read_parameter_file(string const & parameter_path, string& grid_path, string& solution_path,
	string& heuristic_result_path, string& SA_result_path,
	double& captation_radius, double& communication_radius,
	int& reconstruction_threshold, int& SA_iteration_number,
	double& init_temperature, double& decreasing_coef, double& final_temperature);

// Checks if a vertex presents a not dominated neighbor and, if so, adds it to potential_solution_vertices_set
void CheckAndAddToPotentialSolutions(const int & potential_vertex_idx, unordered_set<int>& potential_solution_vertices_set,
	const NeighborGraph & communication_graph, unordered_set<int> const & solution_set);

// Function for removing a vertex from the container stating the dominating potential of each of its neighbors
void RemoveVertexFromNeighborPotentialCovering(const int & vertex_idx, const int & vertex_to_delete_idx,
	unordered_map<int, vector<int> >& covering_potential_map, unordered_set<int>& potential_solution_vertices_set);

// Metaheuristic using the simulated annealing to perform an optimization on a given solution
void SimulatedAnnealingSearch(Solution& solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph,
	const double& init_temperature, const double& final_temperature, const double& decreasing_coef, const int& reconstruction_threshold);

// Local search implementing a swap neighborhood, the modifications were too local so the development was dropped in favor of the neighborhood
// implemented in the simulated annealing
void LocalSearch(Solution solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);