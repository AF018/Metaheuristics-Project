#pragma once

#include <queue>
#include <unordered_map>

#include "NeighborGraphVertex.h"
#include "TargetNet.h"
#include "Solution.h"

class NeighborGraph
{
	std::unordered_map<int, NeighborGraphVertex*> vertices_hashtable;
	// vector including the degree of each vertex for faster access
	std::unordered_map<int, int> degree_hashtable;
	int vertices_number;
	int edges_number;
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
	std::unordered_map<int, int> const & get_degree_hashtable() const;
	int get_vertices_number() const;
	int get_edges_number() const;
	// Check to be used on the communication graph
	bool CheckSolutionConnexity(const Solution& solution);
	// Check to be used on the captation graph
	bool CheckSolutionDomination(const Solution& solution);
	// Removes the vertex and its neighbors from the graph
	void RemoveVertexAndNeighbors(const int& vertex_idx_to_remove);
};

