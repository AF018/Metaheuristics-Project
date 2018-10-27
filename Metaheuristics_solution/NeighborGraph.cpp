#include "NeighborGraph.h"

NeighborGraph::NeighborGraph()
{
	vertices_number = 0;
	edges_vector = vector<vector<int> >();
}

NeighborGraph::NeighborGraph(const TargetNet& target_net, const double& max_distance)
{
	vertices_number = target_net.get_target_number();
	edges_vector = vector<vector<int> >();

	double square_max_dist = pow(max_distance, 2);
	vector<Target> const & target_vect = target_net.get_target_vect();
	vector<Target>::const_iterator it_i = target_vect.begin();
	vector<Target>::const_iterator it_j = target_vect.begin();


	for (it_i = target_vect.begin(); it_i < target_vect.end(); it_i++)
	{
		edges_vector.push_back(vector<int>());
	}

	int edge_number = 0;
	// Creating the edges
	for (it_i = target_vect.begin(); it_i < target_vect.end(); it_i++)
	{
		edges_vector.push_back(vector<int>());
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
	cout << "vertex number : " << vertices_number << endl;
	cout << "edge number : " << edge_number << endl;
}

NeighborGraph::~NeighborGraph()
{
}

vector<vector<int> > const & NeighborGraph::get_edges_vector() const
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

bool NeighborGraph::CheckSolutionConnexity(const Solution& solution, const bool& verbose) const
{
	// Using a BFS to check if the solution set is connex
	// u is for undiscovered, d for discovered and p for processed 
	unordered_map<int, char> idx_to_state_map;
	int processed_vertices_number = 0;
	for (int vertex_idx = 0; vertex_idx < vertices_number; vertex_idx++)
	{
		if (solution.IsVertexInSolution(vertex_idx))
		{
			idx_to_state_map[vertex_idx] = 'u';
		}
	}
	queue<int> bfs_queue;
	int current_vertex_idx;
	// Push the well vertex in the queue
	bfs_queue.push(0);
	while (not bfs_queue.empty())
	{
		current_vertex_idx = bfs_queue.front();
		bfs_queue.pop();
		vector<int> const & vertex_neighbors_vector = edges_vector[current_vertex_idx];
		vector<int>::const_iterator vertex_neighbors_it = vertex_neighbors_vector.begin();
		unordered_map<int, char>::iterator idx_to_state_it;
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

	if (verbose)
	{
		cout << "Found a connex component of size " << processed_vertices_number << "/" << idx_to_state_map.size() << endl;
	}
	return (processed_vertices_number==idx_to_state_map.size());
}

set<int> NeighborGraph::GetNeighbors(vector<int> const & vertex_indices_vector) const
{
	set<int> final_neighbors_set;

	int neighbor_idx;
	vector<int>::const_iterator vertex_indices_vector_it = vertex_indices_vector.begin();
	for (; vertex_indices_vector_it != vertex_indices_vector.end(); vertex_indices_vector_it++)
	{
		vector<int> const & neighbors_vector = (edges_vector.at(*vertex_indices_vector_it));
		vector<int>::const_iterator neighbors_vector_it = neighbors_vector.begin();
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

vector<int> const & NeighborGraph::GetNeighbors(int const & vertex_index) const
{
	return edges_vector.at(vertex_index);
}

bool NeighborGraph::CheckSolutionDomination(const Solution& solution, const bool& verbose) const
{
	// Change to be in accordance with the solution structure
	vector<int> solution_vertex_vector;
	for (int vertex_idx = 0; vertex_idx < vertices_number; vertex_idx++)
	{
		if (solution.IsVertexInSolution(vertex_idx))
		{
			solution_vertex_vector.push_back(vertex_idx);
		}
	}
	set<int> neighbors_vector = GetNeighbors(solution_vertex_vector);

	// Merging the two sets to get the set of covered vertices
	// The two vectors are already sorted
	set<int> union_vector;
	std::merge(solution_vertex_vector.begin(), solution_vertex_vector.end(),
		neighbors_vector.begin(), neighbors_vector.end(),
		std::inserter(union_vector, union_vector.begin()));
	int covered_vertices_number = union_vector.size();

	if (verbose)
	{
		cout << "covered_vertices : " << covered_vertices_number << "/" << vertices_number << endl;
	}
	return (covered_vertices_number == vertices_number);
}