#pragma once

#include <set>

class NeighborGraphVertex
{
	int index;
	std::set<NeighborGraphVertex const*> neighbors_set;
public:
	// Default constructor
	NeighborGraphVertex();
	NeighborGraphVertex(const int& init_index);
	// Default destructor
	~NeighborGraphVertex();
	int get_index() const;
	void AddNeighbor(const NeighborGraphVertex* neighbor_vertex);
	void RemoveNeighbor(const NeighborGraphVertex* neighbor_vertex);
	std::set<NeighborGraphVertex const*> const & get_neighbors_set() const;
};

