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
	std::vector<Target> const & target_vect = target_net.get_target_vect();
	std::vector<Target>::const_iterator it_i = target_vect.begin();
	std::vector<Target>::const_iterator it_j = target_vect.begin();

	// Creating the vertices
	for (it_i = target_vect.begin(); it_i < target_vect.end(); it_i++)
	{
		// The vertex index corresponds to the index in the vector of the target_net
		// In some cases, it might be different from the index contained in the corresponding Target instance
		vertices_vect.push_back(new NeighborGraphVertex(vertices_number));
		degree_vect.push_back(0);
		vertices_number++;
	}

	// Creating the edges
	for (it_i = target_vect.begin(); it_i < target_vect.end(); it_i++)
	{
		for (it_j = std::next(it_i); it_j < target_vect.end(); it_j++)
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

NeighborGraph::NeighborGraph(const NeighborGraph & original_graph)
{
	degree_vect = original_graph.get_degree_vect();
	vertices_number = original_graph.get_vertices_number();
	edges_number = original_graph.get_edges_number();
	// Copying vertices_vector, but by hand because of the pointers
	std::vector<NeighborGraphVertex*> const & original_vertices_vect = original_graph.get_vertices_vect();
	std::vector<NeighborGraphVertex*>::const_iterator vertex_it = original_vertices_vect.begin();
	vertices_vect = std::vector<NeighborGraphVertex*>();
	for (; vertex_it != original_vertices_vect.end(); vertex_it++)
	{
		vertices_vect.push_back(new NeighborGraphVertex(vertices_number));
	}

	// Variable for testing, to be deleted
	int test_edge_count = 0;

	std::set<NeighborGraphVertex const*>::const_iterator neighbors_set_it;
	for (int vertex_idx=0; vertex_idx<vertices_number; vertex_idx++)
	{
		std::set<NeighborGraphVertex const*> const & neighbors_set = (original_vertices_vect[vertex_idx])->get_neighbors_set();
		for (neighbors_set_it = neighbors_set.begin(); neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			// Adding to the vertex of the new instance the same neighbors that the original vertex has
			int neighbor_idx = (*neighbors_set_it)->get_index();
			vertices_vect[vertex_idx]->AddNeighbor(vertices_vect[neighbor_idx]);
			test_edge_count++;
		}
	}
	std::cout << "Copy just done, number of edges : " << test_edge_count/2 << std::endl;
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

std::vector<NeighborGraphVertex*> const & NeighborGraph::get_vertices_vect() const
{
	return vertices_vect;
}

std::vector<int> const & NeighborGraph::get_degree_vect() const
{
	return degree_vect;
}

int NeighborGraph::get_vertices_number() const
{
	return vertices_number;
}

int NeighborGraph::get_edges_number() const
{
	return edges_number;
}

bool NeighborGraph::CheckSolutionConnexity(const Solution& solution)
{
	// Using a BFS to check if the solution set is connex
	// u is for undiscovered, d for discovered and p for processed 
	std::map<int, char> idx_to_state_map;
	int processed_vertices_number = 0;
	for (int vertex_idx = 0; vertex_idx < vertices_number; vertex_idx++)
	{
		if (solution.IsVertexInSolution(vertex_idx))
		{
			idx_to_state_map[vertex_idx] = 'u';
		}
	}
	std::queue<NeighborGraphVertex const *> bfs_queue;
	NeighborGraphVertex const * current_vertex;
	// Push one element in the queue
	bfs_queue.push(vertices_vect[(idx_to_state_map.begin()->first)]);
	while (not bfs_queue.empty())
	{
		current_vertex = bfs_queue.front();
		bfs_queue.pop();
		std::set<NeighborGraphVertex const*> const & vertex_neighbors_set = current_vertex->get_neighbors_set();
		std::set<NeighborGraphVertex const*>::const_iterator vertex_neighbors_it = vertex_neighbors_set.begin();
		std::map<int, char>::iterator idx_to_state_it;
		for (; vertex_neighbors_it != vertex_neighbors_set.end(); vertex_neighbors_it++)
		{
			int neighbor_idx = (*vertex_neighbors_it)->get_index();
			idx_to_state_it = idx_to_state_map.find(neighbor_idx);
			if (idx_to_state_it != idx_to_state_map.end())
			{
				// Only process if the vertex is part of the solution
				char vertex_state = idx_to_state_it->second;
				if (vertex_state == 'u')
				{
					vertex_state = 'd';
					bfs_queue.push(*vertex_neighbors_it);
				}
				// Do nothing if the vertex has been discovered or processed
			}
		}
		idx_to_state_map[current_vertex->get_index()] = 'p';
		processed_vertices_number++;
	}
	std::cout << "found a connex component of size " << processed_vertices_number << "/" << idx_to_state_map.size() << std::endl;
	return (processed_vertices_number==idx_to_state_map.size());
}

bool NeighborGraph::CheckSolutionDomination(const Solution& solution)
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
			std::set<NeighborGraphVertex const*> const & vertex_neighbors_set = vertices_vect[vertex_idx]->get_neighbors_set();
			std::set<NeighborGraphVertex const*>::const_iterator vertex_neighbors_it = vertex_neighbors_set.begin();
			for (; vertex_neighbors_it!=vertex_neighbors_set.end(); vertex_neighbors_it++)
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