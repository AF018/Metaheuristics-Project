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
	std::set<NeighborGraphVertex const*> const & get_neighbors_set() const;
	int get_degree() const;
	void AddEdge(const NeighborGraphVertex* neighbor_vertex);
	void RemoveEdge(const NeighborGraphVertex* neighbor_vertex);
};

