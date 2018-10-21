#pragma once

#include "NeighborGraphVertex.h"
#include "TargetNet.h"

class NeighborGraph
{
	std::vector<NeighborGraphVertex> vertices_vect;
	std::vector<int> degree_vect;
	int vertices_number;
	int edges_number;
public:
	// Default constructor
	NeighborGraph();
	// Builds a graph based on target_net, placing edges between targets that are closer than max_distance
	NeighborGraph(const TargetNet& target_net, const double& max_distance);
	virtual ~NeighborGraph();
};

