#include "NeighborGraphVertex.h"

NeighborGraphVertex::NeighborGraphVertex()
{
}

NeighborGraphVertex::NeighborGraphVertex(const int& init_index)
{
	index = init_index;
	neighbors_set = std::set<int>();
}

NeighborGraphVertex::~NeighborGraphVertex()
{
}

void NeighborGraphVertex::AddNeighbor(const int& neighbor_idx)
{
	neighbors_set.insert(neighbor_idx);
}