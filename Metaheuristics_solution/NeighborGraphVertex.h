#pragma once

#include <set>

class NeighborGraphVertex
{
	int index;
	std::set<int> neighbors_set;
public:
	NeighborGraphVertex();
	NeighborGraphVertex(const int& init_index);
	~NeighborGraphVertex();
	void AddNeighbor(const int& neighbor_idx);
};

