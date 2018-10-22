#include "NeighborGraph.h"

NeighborGraph::NeighborGraph()
{
	vertices_vect = std::vector<NeighborGraphVertex*>();
	degree_vect = std::vector<int>();
	vertices_number = 0;
	edges_number = 0;
}

NeighborGraph::NeighborGraph(const TargetNet& target_net, const double& max_distance)
{
	vertices_number = 0;

	double square_max_dist = pow(max_distance, 2);
	std::vector<Target> const * target_vect = target_net.get_target_vect();
	std::vector<Target>::const_iterator it_i = target_vect->begin();
	std::vector<Target>::const_iterator it_j = target_vect->begin();

	// Creating the vertices
	for (it_i = target_vect->begin(); it_i < target_vect->end(); it_i++)
	{
		// The vertex index corresponds to the index in the vector of the target_net
		// In some cases, it might be different from the index contained in the corresponding Target instance
		vertices_vect.push_back(new NeighborGraphVertex(vertices_number));
		degree_vect.push_back(0);
		vertices_number++;
	}

	// Creating the edges
	for (it_i = target_vect->begin(); it_i < target_vect->end(); it_i++)
	{
		for (it_j = std::next(it_i); it_j < target_vect->end(); it_j++)
		{
			// Computing the distance between i and j
			double i_j_dist = pow(it_i->get_x_coord() - it_j->get_x_coord(), 2)
							+ pow(it_i->get_y_coord() - it_j->get_y_coord(), 2);
			if (i_j_dist <= square_max_dist)
			{
				// Adding the edge in both directions (undirected graph) and incrementing the degrees
				vertices_vect[it_i->get_index()]->AddNeighbor(vertices_vect[it_j->get_index()]);
				degree_vect[it_i->get_index()] += 1;
				vertices_vect[it_j->get_index()]->AddNeighbor(vertices_vect[it_i->get_index()]);
				degree_vect[it_j->get_index()] += 1;
				edges_number++;
			}
		}
	}
	std::cout << "vertex number : " << vertices_number << std::endl;
	std::cout << "edge number : " << edges_number << std::endl;
}

NeighborGraph::~NeighborGraph()
{
	// Pointers to NeighborGraphVertex in the vector vertices_vect need to be deleted
	std::vector<NeighborGraphVertex*>::iterator vertex_it = vertices_vect.begin();
	for (; vertex_it < vertices_vect.end(); vertex_it++)
	{
		delete (*vertex_it);
	}
}

bool NeighborGraph::CheckSolutionConnexity(const Solution& solution)
{
	return false;
}

bool NeighborGraph::CheckSolutionDominance(const Solution& solution)
{
	// Vector telling if each vertex is covered by the solution
	int covered_vertices_number = 0;
	std::vector<bool> covering_vect(vertices_number, 0);
	for (int vertex_idx=0; vertex_idx<vertices_number; vertex_idx++)
	{
		if (solution.IsVertexInSolution(vertex_idx))
		{
			if (not covering_vect[vertex_idx])
			{
				covering_vect[vertex_idx] = true;
				covered_vertices_number++;
			}
			std::set<NeighborGraphVertex const*> const * vertex_neighbors_set = vertices_vect[vertex_idx]->get_neighbors_set();
			std::set<NeighborGraphVertex const*>::const_iterator vertex_neighbors_it = vertex_neighbors_set->begin();
			for (; vertex_neighbors_it!=vertex_neighbors_set->end(); vertex_neighbors_it++)
			{
				int neighbor_idx = (*vertex_neighbors_it)->get_index();
				if (not covering_vect[neighbor_idx])
				{
					covering_vect[neighbor_idx] = true;
					covered_vertices_number++;
				}
			}
		}
	}
	std::cout << "covered_vertices : " << covered_vertices_number << "/" << vertices_number << std::endl;
	return (covered_vertices_number == vertices_number);
}