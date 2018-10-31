#pragma once

#include <algorithm>
#include <iterator>
#include <queue>
#include <unordered_map>
#include <set>

#include "TargetNet.h"
#include "Solution.h"

using namespace std;

class NeighborGraph
{
	int vertices_number;
	vector<vector<int> > edges_vector;
public:
	// Default constructor
	NeighborGraph();
	// Builds a graph based on target_net, placing edges between targets that are closer than max_distance
	NeighborGraph(const TargetNet& target_net, const double& max_distance);
	// Default destructor
	virtual ~NeighborGraph();
	vector<vector<int> > const & get_edges_vector() const;
	int get_vertices_number() const;
	int get_vertex_degree(int const & vertex_idx) const;
	vector<int> ComputeConnexComponent(unordered_set<int>& vertices_to_process_set, const int& start_vertex_idx) const;
	vector<vector<int> > ComputeConnexComponents(const Solution& solution) const;
	// Check to be used on the communication graph
	bool CheckSolutionConnexity(const Solution& solution, const bool& verbose=false) const;
	// Returns the set of neighbors of a given vertex indices set
	set<int> GetNeighbors(vector<int> const & vertex_indices_vector) const;
	// Returns the set of neighbors of a given vertex index
	vector<int> const & GetNeighbors(int const & vertex_index) const;
	// Check to be used on the captation graph
	bool CheckSolutionDomination(const Solution& solution, const bool& verbose=false) const;
};

