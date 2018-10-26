#include "NeighborGraph.h"

NeighborGraph::NeighborGraph()
{
	vertices_number = 0;
	edges_vector = std::vector<std::vector<int> >();
}

NeighborGraph::NeighborGraph(const TargetNet& target_net, const double& max_distance)
{
	vertices_number = target_net.get_target_number();
	edges_vector = std::vector<std::vector<int> >();

	double square_max_dist = pow(max_distance, 2);
	std::vector<Target> const & target_vect = target_net.get_target_vect();
	std::vector<Target>::const_iterator it_i = target_vect.begin();
	std::vector<Target>::const_iterator it_j = target_vect.begin();


	for (it_i = target_vect.begin(); it_i < target_vect.end(); it_i++)
	{
		edges_vector.push_back(std::vector<int>());
	}

	int edge_number = 0;
	// Creating the edges
	for (it_i = target_vect.begin(); it_i < target_vect.end(); it_i++)
	{
		edges_vector.push_back(std::vector<int>());
		for (it_j = std::next(it_i); it_j < target_vect.end(); it_j++)
		{
			// Computing the distance between i and j
			double i_j_dist = pow(it_i->get_x_coord() - it_j->get_x_coord(), 2)
							+ pow(it_i->get_y_coord() - it_j->get_y_coord(), 2);
			if (i_j_dist <= square_max_dist)
			{
				// Adding the edge
				edges_vector[it_i->get_index()].push_back(it_j->get_index());
				edges_vector[it_j->get_index()].push_back(it_i->get_index());
				edge_number++;
			}
		}
	}
	std::cout << "vertex number : " << vertices_number << std::endl;
	std::cout << "edge number : " << edge_number << std::endl;
}

NeighborGraph::~NeighborGraph()
{
}

std::vector<std::vector<int> > const & NeighborGraph::get_edges_vector() const
{
	return edges_vector;
}

int NeighborGraph::get_vertices_number() const
{
	return vertices_number;
}

int NeighborGraph::get_vertex_degree(int const & vertex_idx) const
{
	return edges_vector.at(vertex_idx).size();
}

bool NeighborGraph::CheckSolutionConnexity(const Solution& solution) const
{
	// Using a BFS to check if the solution set is connex
	// u is for undiscovered, d for discovered and p for processed 
	std::unordered_map<int, char> idx_to_state_map;
	int processed_vertices_number = 0;
	for (int vertex_idx = 0; vertex_idx < vertices_number; vertex_idx++)
	{
		if (solution.IsVertexInSolution(vertex_idx))
		{
			idx_to_state_map[vertex_idx] = 'u';
		}
	}
	std::queue<int> bfs_queue;
	int current_vertex_idx;
	// Push the well vertex in the queue
	bfs_queue.push(0);
	while (not bfs_queue.empty())
	{
		current_vertex_idx = bfs_queue.front();
		bfs_queue.pop();
		std::vector<int> const & vertex_neighbors_vector = edges_vector[current_vertex_idx];
		std::vector<int>::const_iterator vertex_neighbors_it = vertex_neighbors_vector.begin();
		std::unordered_map<int, char>::iterator idx_to_state_it;
		for (; vertex_neighbors_it != vertex_neighbors_vector.end(); vertex_neighbors_it++)
		{
			int neighbor_idx = *vertex_neighbors_it;
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
		idx_to_state_map[current_vertex_idx] = 'p';
		processed_vertices_number++;
	}
	std::cout << "Found a connex component of size " << processed_vertices_number << "/" << idx_to_state_map.size()  << std::endl;
	return (processed_vertices_number==idx_to_state_map.size());
}

std::set<int> NeighborGraph::GetNeighbors(std::vector<int> const & vertex_indices_vector) const
{
	std::set<int> final_neighbors_set;

	int neighbor_idx;
	std::vector<int>::const_iterator vertex_indices_vector_it = vertex_indices_vector.begin();
	for (; vertex_indices_vector_it != vertex_indices_vector.end(); vertex_indices_vector_it++)
	{
		std::vector<int> const & neighbors_vector = (edges_vector.at(*vertex_indices_vector_it));
		std::vector<int>::const_iterator neighbors_vector_it = neighbors_vector.begin();
		for (; neighbors_vector_it != neighbors_vector.end(); neighbors_vector_it++)
		{
			neighbor_idx = *neighbors_vector_it;
			final_neighbors_set.insert(neighbor_idx);
		}
	}

	// Removing the solution vertices from the neighborhood
	//vertex_indices_vector_it = vertex_indices_vector.begin();
	//for (; vertex_indices_vector_it != vertex_indices_vector.end(); vertex_indices_vector_it++)
	//{
	//	final_neighbors_set.erase(*vertex_indices_vector_it);
	//}
	// Sorting just to make sure, needed for the heuristic
	//std::sort(final_neighbors_set.begin(), final_neighbors_set.end());
	return final_neighbors_set;
}

std::vector<int> const & NeighborGraph::GetNeighbors(int const & vertex_index) const
{
	return edges_vector.at(vertex_index);
}

bool NeighborGraph::CheckSolutionDomination(const Solution& solution) const
{
	std::vector<int> solution_vertex_vector;
	for (int vertex_idx = 0; vertex_idx < vertices_number; vertex_idx++)
	{
		if (solution.IsVertexInSolution(vertex_idx))
		{
			solution_vertex_vector.push_back(vertex_idx);
		}
	}
	std::set<int> neighbors_vector = GetNeighbors(solution_vertex_vector);

	// Merging the two sets to get the set of covered vertices
	// The two vectors are already sorted
	std::set<int> union_vector;
	std::merge(solution_vertex_vector.begin(), solution_vertex_vector.end(),
		neighbors_vector.begin(), neighbors_vector.end(),
		std::inserter(union_vector, union_vector.begin()));
	int covered_vertices_number = union_vector.size();

	std::cout << "covered_vertices : " << covered_vertices_number << "/" << vertices_number << std::endl;
	return (covered_vertices_number == vertices_number);
}

//std::set<int> NeighborGraph::HeuristicRemoval(const int & vertex_idx_to_remove)
//{
//	std::set<int> neighbors_idx_set;
//	std::cout << "index to remove : " << vertex_idx_to_remove << std::endl;
//	std::cout << "size of its neighbor set : " << vertices_hashtable[vertex_idx_to_remove]->get_neighbors_set().size() << std::endl;
//
//	std::set<NeighborGraphVertex const *> const & neighbors_set = vertices_hashtable[vertex_idx_to_remove]->get_neighbors_set();
//	std::set<NeighborGraphVertex const *>::const_iterator neighbors_set_it = neighbors_set.begin();
//	int neighbor_idx = 0;
//	NeighborGraphVertex const * neighbor_vertex = nullptr;
//	for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
//	{
//		neighbor_vertex = *neighbors_set_it;
//		neighbor_idx = neighbor_vertex->get_index();
//		neighbors_idx_set.insert(neighbor_idx);
//		if (neighbor_vertex->get_degree() > 0)
//		{
//			std::set<NeighborGraphVertex const *> const & neighbors_of_neighbor_set = neighbor_vertex->get_neighbors_set();
//			std::set<NeighborGraphVertex const *>::const_iterator neighbors_of_neighbor_set_it = neighbors_of_neighbor_set.begin();
//			// Removing each neighbor vertex from all its own neighbors' neighbor sets
//			for (; neighbors_of_neighbor_set_it != neighbors_of_neighbor_set.end(); neighbors_of_neighbor_set_it++)
//			{
//				int neighbor_of_neighbor_idx = (*neighbors_of_neighbor_set_it)->get_index();
//				if (neighbor_of_neighbor_idx != vertex_idx_to_remove)
//				{
//					std::cout << "neighbor idx : " << neighbor_idx << "  neighbor of neighbor idx : " << neighbor_of_neighbor_idx << std::endl;
//					// Check to see if the neighbor of neighbor is not the original vertex to remove
//					// Messes up with the deletion process if we remove its neighbors now
//					vertices_hashtable[neighbor_of_neighbor_idx]->RemoveEdge(neighbor_vertex);
//				}
//			}
//		}
//	}
//	// Deleting the edges pointing towards each element of the neighbors_set
//	for (neighbors_set_it = neighbors_set.begin(); neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
//	{
//		// Every edge pointing to the vertex to remove is removed
//		// This cannot be done before or it messes with the neighbor container traversal
//		neighbor_vertex = *neighbors_set_it;
//		neighbor_idx = neighbor_vertex->get_index();
//		//vertices_hashtable[neighbor_idx]->RemoveEdge(vertices_hashtable[vertex_idx_to_remove]);
//
//		vertices_hashtable.erase(neighbor_idx);
//		vertices_number--;
//		std::cout << "erased the neighbor : " << neighbor_idx << std::endl;
//		delete neighbor_vertex;
//		std::cout << "deleted the neighbor pointer" << std::endl;
//
//	}
//	// Deleting the original vertex
//	vertices_number--;
//	delete vertices_hashtable[vertex_idx_to_remove];
//	vertices_hashtable.erase(vertex_idx_to_remove);
//	std::cout << "Vertices number : " << vertices_number << std::endl;
//	return neighbors_idx_set;
//}