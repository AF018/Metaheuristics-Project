#pragma once

#include <algorithm>
#include <iterator>
#include <queue>
#include <unordered_map>
#include <set>

//#include "NeighborGraphVertex.h"
#include "TargetNet.h"
#include "Solution.h"

class NeighborGraph
{
	int vertices_number;
	std::vector<std::vector<int> > edges_vector;
public:
	// Default constructor
	NeighborGraph();
	// Builds a graph based on target_net, placing edges between targets that are closer than max_distance
	NeighborGraph(const TargetNet& target_net, const double& max_distance);
	// Default destructor
	virtual ~NeighborGraph();
	std::vector<std::vector<int> > const & get_edges_vector() const;
	int get_vertices_number() const;
	int get_vertex_degree(int const & vertex_idx) const;
	// Check to be used on the communication graph
	bool CheckSolutionConnexity(const Solution& solution) const;
	// Returns the set of neighbors of a given vertex indices set
	std::set<int> GetNeighbors(std::vector<int> const & vertex_indices_vector) const;
	// Returns the set of neighbors of a given vertex index
	std::vector<int> const & GetNeighbors(int const & vertex_index) const;
	// Check to be used on the captation graph
	bool CheckSolutionDomination(const Solution& solution) const;
	// Removes the vertex and the incoming edges of its neighbors from the graph
	// Returns a set of these neighbors
	//std::set<int> HeuristicRemoval(const int& vertex_idx_to_remove);
};

