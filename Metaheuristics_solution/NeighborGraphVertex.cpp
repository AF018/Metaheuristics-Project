#include "NeighborGraphVertex.h"

NeighborGraphVertex::NeighborGraphVertex()
{
}

NeighborGraphVertex::NeighborGraphVertex(const int& init_index)
{
	index = init_index;
	neighbors_set = std::set<NeighborGraphVertex const*>();
}

NeighborGraphVertex::~NeighborGraphVertex()
{
}

int NeighborGraphVertex::get_index() const
{
	return index;
}

std::set<NeighborGraphVertex const*> const & NeighborGraphVertex::get_neighbors_set() const
{
	return neighbors_set;
}

int NeighborGraphVertex::get_degree() const
{
	return neighbors_set.size();
}


void NeighborGraphVertex::AddEdge(const NeighborGraphVertex* neighbor_vertex)
{
	neighbors_set.insert(neighbor_vertex);
}

void NeighborGraphVertex::RemoveEdge(const NeighborGraphVertex * neighbor_vertex)
{
	neighbors_set.erase(neighbor_vertex);
}
