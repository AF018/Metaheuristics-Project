#include "NeighborGraph.h"

NeighborGraph::NeighborGraph()
{
	vertices_hashtable = std::unordered_map<int, NeighborGraphVertex*>();
	degree_hashtable = std::unordered_map<int, int>();
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
		vertices_hashtable[vertices_number] = new NeighborGraphVertex(vertices_number);
		degree_hashtable[vertices_number] = 0;
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
				vertices_hashtable[it_i->get_index()]->AddNeighbor(vertices_hashtable[it_j->get_index()]);
				degree_hashtable[it_i->get_index()] += 1;
				vertices_hashtable[it_j->get_index()]->AddNeighbor(vertices_hashtable[it_i->get_index()]);
				degree_hashtable[it_j->get_index()] += 1;
				edges_number++;
			}
		}
	}
	std::cout << "vertex number : " << vertices_number << std::endl;
	std::cout << "edge number : " << edges_number << std::endl;
}

NeighborGraph::NeighborGraph(const NeighborGraph & original_graph)
{
	degree_hashtable = original_graph.get_degree_hashtable();
	vertices_number = 0;
	edges_number = original_graph.get_edges_number();
	// Copying vertices_hashtable, but by hand because of the pointers
	std::unordered_map<int, NeighborGraphVertex*> const & original_vertices_hashtable = original_graph.get_vertices_hashtable();
	std::unordered_map<int, NeighborGraphVertex*>::const_iterator vertex_it = original_vertices_hashtable.begin();
	vertices_hashtable = std::unordered_map<int, NeighborGraphVertex*>();
	for (; vertex_it != original_vertices_hashtable.end(); vertex_it++)
	{
		vertices_hashtable[vertices_number] = new NeighborGraphVertex(vertices_number);
		vertices_number++;
	}

	// Variable for testing, to be deleted
	int test_edge_count = 0;

	std::set<NeighborGraphVertex const*>::const_iterator neighbors_set_it;
	for (int vertex_idx=0; vertex_idx<vertices_number; vertex_idx++)
	{
		std::set<NeighborGraphVertex const*> const & neighbors_set = original_vertices_hashtable.at(vertex_idx)->get_neighbors_set();
		for (neighbors_set_it = neighbors_set.begin(); neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			// Adding to the vertex of the new instance the same neighbors that the original vertex has
			int neighbor_idx = (*neighbors_set_it)->get_index();
			vertices_hashtable[vertex_idx]->AddNeighbor(vertices_hashtable[neighbor_idx]);
			test_edge_count++;
		}
	}
	std::cout << "Copy just done, number of edges : " << test_edge_count / 2 << std::endl;
	std::cout << "And the number of vertices : " << vertices_number << std::endl;
}

NeighborGraph::~NeighborGraph()
{
	// Pointers to NeighborGraphVertex in the vector vertices_hashtable need to be deleted
	std::unordered_map<int, NeighborGraphVertex*>::iterator vertex_it = vertices_hashtable.begin();
	for (; vertex_it != vertices_hashtable.end(); vertex_it++)
	{
		delete vertex_it->second;
	}
}

std::unordered_map<int, NeighborGraphVertex*> const & NeighborGraph::get_vertices_hashtable() const
{
	return vertices_hashtable;
}

std::unordered_map<int, int> const & NeighborGraph::get_degree_hashtable() const
{
	return degree_hashtable;
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
	bfs_queue.push(vertices_hashtable[(idx_to_state_map.begin()->first)]);
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
			std::set<NeighborGraphVertex const*> const & vertex_neighbors_set = vertices_hashtable[vertex_idx]->get_neighbors_set();
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

void NeighborGraph::RemoveVertexAndNeighbors(const int & vertex_idx_to_remove)
{
	std::cout << "Hi we're in" << std::endl;
	std::cout << "index to remove : " << vertex_idx_to_remove << std::endl;

	std::set<NeighborGraphVertex const *> const & neighbors_set = vertices_hashtable[vertex_idx_to_remove]->get_neighbors_set();
	std::set<NeighborGraphVertex const *>::const_iterator neighbors_set_it = neighbors_set.begin();
	int neighbor_idx = 0;
	NeighborGraphVertex const * neighbor_vertex = nullptr;
	for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
	{
		std::cout << "pointer : " << *neighbors_set_it << std::endl;
		std::cout << "index : " << (*neighbors_set_it)->get_index() << std::endl;
		neighbor_vertex = *neighbors_set_it;
		neighbor_idx = neighbor_vertex->get_index();
		std::set<NeighborGraphVertex const *> const & neighbors_of_neighbor_set = neighbor_vertex->get_neighbors_set();
		std::set<NeighborGraphVertex const *>::const_iterator neighbors_of_neighbor_set_it = neighbors_of_neighbor_set.begin();
		// Removing each neighbor vertex from all its own neighbors' neighbor sets
		for (; neighbors_of_neighbor_set_it != neighbors_of_neighbor_set.end(); neighbors_of_neighbor_set_it++)
		{
			int neighbor_of_neighbor_idx = (*neighbors_of_neighbor_set_it)->get_index();
			// Check to see if the neighbor of neighbor is not the original vertex to remove
			// Messes up with the deletion process if we remove its neighbors now
			if (neighbor_of_neighbor_idx != vertex_idx_to_remove)
			{
				std::cout << "In the neighbor neighbor loop : " << *neighbors_of_neighbor_set_it << std::endl;
				std::cout << "and its label                 : " << (*neighbors_of_neighbor_set_it)->get_index() << std::endl;
				vertices_hashtable[neighbor_of_neighbor_idx]->RemoveNeighbor(neighbor_vertex);
				edges_number--;
			}
		}
		std::cout << "Got out of the loop" << std::endl;
	}
	// Deleting the neighbors
	for (neighbors_set_it = neighbors_set.begin(); neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
	{
		neighbor_vertex = *neighbors_set_it;
		neighbor_idx = neighbor_vertex->get_index();
		vertices_hashtable.erase(neighbor_idx);
		degree_hashtable.erase(neighbor_idx);
		vertices_number--;
		std::cout << "erased the neighbor : " << neighbor_idx << std::endl;
		delete neighbor_vertex;
		std::cout << "deleted the neighbor pointer" << std::endl;
	}
	// Decrementing edges_number from the number of neighbors (i.e. edges) the vertex to remove had
	edges_number -= degree_hashtable[vertex_idx_to_remove];
	vertices_number--;
	// Deleting the original vertex
	delete vertices_hashtable[vertex_idx_to_remove];
	vertices_hashtable.erase(vertex_idx_to_remove);
	degree_hashtable.erase(vertex_idx_to_remove);
	std::cout << "Size of the neighborhood, should be zero : " << neighbors_set.size() << std::endl;
	std::cout << "Vertices number : " << vertices_number << "   Edges number : " << edges_number << std::endl;
}
