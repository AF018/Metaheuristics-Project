#pragma once

#include "NeighborGraphVertex.h"
#include "TargetNet.h"
#include "Solution.h"

class NeighborGraph
{
	std::vector<NeighborGraphVertex*> vertices_vect;
	// vector including the degree of each vertex for faster access
	std::vector<int> degree_vect;
	int vertices_number;
	int edges_number;
public:
	// Default constructor
	NeighborGraph();
	// Builds a graph based on target_net, placing edges between targets that are closer than max_distance
	NeighborGraph(const TargetNet& target_net, const double& max_distance);
	// Default destructor
	virtual ~NeighborGraph();
	// Check function to be used on the communication graph
	bool CheckSolutionConnexity(const Solution& solution);
	// Check function to be used on the captation graph
	bool CheckSolutionDominance(const Solution& solution);
};

