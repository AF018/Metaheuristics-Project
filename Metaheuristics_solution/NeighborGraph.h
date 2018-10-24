#pragma once

#include <algorithm>
#include <iterator>
#include <queue>
#include <unordered_map>

#include "NeighborGraphVertex.h"
#include "TargetNet.h"
#include "Solution.h"

class NeighborGraph
{
	std::unordered_map<int, NeighborGraphVertex*> vertices_hashtable;
	int vertices_number;
public:
	// Default constructor
	NeighborGraph();
	// Builds a graph based on target_net, placing edges between targets that are closer than max_distance
	NeighborGraph(const TargetNet& target_net, const double& max_distance);
	// Copy constructor
	NeighborGraph(const NeighborGraph& original_graph);
	// Default destructor
	virtual ~NeighborGraph();
	std::unordered_map<int, NeighborGraphVertex*> const & get_vertices_hashtable() const;
	int get_vertices_number() const;
	int get_vertex_degree(int const & vertex_idx) const;
	// Check to be used on the communication graph
	bool CheckSolutionConnexity(const Solution& solution) const;
	// Returns the set of neighbors of a given vertex indices set
	std::set<int> GetNeighbors(std::set<int> const & vertex_indices_set) const;
	// Check to be used on the captation graph
	bool CheckSolutionDomination(const Solution& solution) const;
	// Removes the vertex and the incoming edges of its neighbors from the graph
	// Returns a set of these neighbors
	std::set<int> HeuristicRemoval(const int& vertex_idx_to_remove);
};

