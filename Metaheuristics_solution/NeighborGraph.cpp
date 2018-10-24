#include "NeighborGraph.h"

NeighborGraph::NeighborGraph()
{
	vertices_hashtable = std::unordered_map<int, NeighborGraphVertex*>();
	vertices_number = 0;
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
				// Adding the edge in both directions (undirected graph)
				vertices_hashtable[it_i->get_index()]->AddEdge(vertices_hashtable[it_j->get_index()]);
				vertices_hashtable[it_j->get_index()]->AddEdge(vertices_hashtable[it_i->get_index()]);
			}
		}
	}
	std::cout << "vertex number : " << vertices_number << std::endl;
}

NeighborGraph::NeighborGraph(const NeighborGraph & original_graph)
{
	vertices_number = 0;
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
			vertices_hashtable[vertex_idx]->AddEdge(vertices_hashtable[neighbor_idx]);
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

int NeighborGraph::get_vertices_number() const
{
	return vertices_number;
}

int NeighborGraph::get_vertex_degree(int const & vertex_idx) const
{
	return vertices_hashtable.at(vertex_idx)->get_degree();
}

bool NeighborGraph::CheckSolutionConnexity(const Solution& solution) const
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
	bfs_queue.push(vertices_hashtable.at(idx_to_state_map.begin()->first));
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
				if (idx_to_state_it->second == 'u')
				{
					idx_to_state_it->second = 'd';
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

std::set<int> NeighborGraph::GetNeighbors(std::set<int> const & vertex_indices_set) const
{
	std::set<int> final_neighbors_set;

	int neighbor_idx;
	std::set<int>::const_iterator vertex_indices_set_it = vertex_indices_set.begin();
	for (; vertex_indices_set_it != vertex_indices_set.end(); vertex_indices_set_it++)
	{
		std::set<NeighborGraphVertex const *> const & neighbors_set = (vertices_hashtable.at(*vertex_indices_set_it)->get_neighbors_set());
		std::set<NeighborGraphVertex const *>::const_iterator neighbors_set_it = neighbors_set.begin();
		for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			neighbor_idx = (*neighbors_set_it)->get_index();
			if (vertex_indices_set.find(neighbor_idx) == vertex_indices_set.end())
			{
				final_neighbors_set.insert(neighbor_idx);
			}
		}
	}
	return final_neighbors_set;
}

bool NeighborGraph::CheckSolutionDomination(const Solution& solution) const
{
	std::set<int> solution_vertex_set;
	for (int vertex_idx = 0; vertex_idx < vertices_number; vertex_idx++)
	{
		if (solution.IsVertexInSolution(vertex_idx))
		{
			solution_vertex_set.insert(vertex_idx);
		}
	}
	std::set<int> neighbors_set = GetNeighbors(solution_vertex_set);

	// Merging the two sets to get the set of covered vertices
	std::set<int> union_set;
	std::merge(solution_vertex_set.begin(), solution_vertex_set.end(),
		neighbors_set.begin(), neighbors_set.end(),
		std::inserter(union_set, union_set.begin()));
	int covered_vertices_number = union_set.size();

	std::cout << "covered_vertices : " << covered_vertices_number << "/" << vertices_number << std::endl;
	return (covered_vertices_number == vertices_number);
}

std::set<int> NeighborGraph::HeuristicRemoval(const int & vertex_idx_to_remove)
{
	std::set<int> neighbors_idx_set;
	std::cout << "index to remove : " << vertex_idx_to_remove << std::endl;
	std::cout << "size of its neighbor set : " << vertices_hashtable[vertex_idx_to_remove]->get_neighbors_set().size() << std::endl;

	std::set<NeighborGraphVertex const *> const & neighbors_set = vertices_hashtable[vertex_idx_to_remove]->get_neighbors_set();
	std::set<NeighborGraphVertex const *>::const_iterator neighbors_set_it = neighbors_set.begin();
	int neighbor_idx = 0;
	NeighborGraphVertex const * neighbor_vertex = nullptr;
	for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
	{
		neighbor_vertex = *neighbors_set_it;
		neighbor_idx = neighbor_vertex->get_index();
		neighbors_idx_set.insert(neighbor_idx);
		if (neighbor_vertex->get_degree() > 0)
		{
			std::set<NeighborGraphVertex const *> const & neighbors_of_neighbor_set = neighbor_vertex->get_neighbors_set();
			std::set<NeighborGraphVertex const *>::const_iterator neighbors_of_neighbor_set_it = neighbors_of_neighbor_set.begin();
			// Removing each neighbor vertex from all its own neighbors' neighbor sets
			for (; neighbors_of_neighbor_set_it != neighbors_of_neighbor_set.end(); neighbors_of_neighbor_set_it++)
			{
				int neighbor_of_neighbor_idx = (*neighbors_of_neighbor_set_it)->get_index();
				if (neighbor_of_neighbor_idx != vertex_idx_to_remove)
				{
					std::cout << "neighbor idx : " << neighbor_idx << "  neighbor of neighbor idx : " << neighbor_of_neighbor_idx << std::endl;
					// Check to see if the neighbor of neighbor is not the original vertex to remove
					// Messes up with the deletion process if we remove its neighbors now
					vertices_hashtable[neighbor_of_neighbor_idx]->RemoveEdge(neighbor_vertex);
				}
			}
		}
	}
	// Deleting the edges pointing towards each element of the neighbors_set
	for (neighbors_set_it = neighbors_set.begin(); neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
	{
		// Every edge pointing to the vertex to remove is removed
		// This cannot be done before or it messes with the neighbor container traversal
		neighbor_vertex = *neighbors_set_it;
		neighbor_idx = neighbor_vertex->get_index();
		vertices_hashtable[neighbor_idx]->RemoveEdge(vertices_hashtable[vertex_idx_to_remove]);
	}
	// Deleting the original vertex
	vertices_number--;
	delete vertices_hashtable[vertex_idx_to_remove];
	vertices_hashtable.erase(vertex_idx_to_remove);
	std::cout << "Vertices number : " << vertices_number << std::endl;
	return neighbors_idx_set;
}
