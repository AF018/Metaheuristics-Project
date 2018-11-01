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
	cout << edges_vector.size() << endl;
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

vector<int> NeighborGraph::ComputeConnexComponent(unordered_set<int>& vertices_to_process_set, const int & start_vertex_idx) const
{
	// Deleting from the set the start vertex so it doesn't get counted twice
	vertices_to_process_set.erase(start_vertex_idx);
	// Using a BFS to get the connex component
	queue<int> bfs_queue;
	int current_vertex_idx;
	vector<int> connex_component;
		
	bfs_queue.push(start_vertex_idx);
	while (not bfs_queue.empty())
	{
		current_vertex_idx = bfs_queue.front();
		bfs_queue.pop();
		vector<int> const & vertex_neighbors_vector = edges_vector[current_vertex_idx];
		vector<int>::const_iterator vertex_neighbors_it = vertex_neighbors_vector.begin();
		for (; vertex_neighbors_it != vertex_neighbors_vector.end(); vertex_neighbors_it++)
		{
			int neighbor_idx = *vertex_neighbors_it;
			if (vertices_to_process_set.count(neighbor_idx) == 1)
			{
				vertices_to_process_set.erase(neighbor_idx);
				bfs_queue.push(neighbor_idx);
			}
		}
		connex_component.push_back(current_vertex_idx);
	}
	return connex_component;
}

vector<vector<int> > NeighborGraph::ComputeConnexComponents(const Solution & solution) const
{
	vector<vector<int>> connex_components = vector<vector<int> >();
	// Set containing all the solution vertices not yet encountered
	unordered_set<int> not_processed_vertices_set = solution.get_solution_set();

	// Using a BFS to check if the solution set is connex
	queue<int> bfs_queue;
	int start_idx;
	while (not_processed_vertices_set.size() > 0)
	{
		if (connex_components.size() == 0)
		{
			// Push the well vertex in the queue
			start_idx = 0;
		}
		else
		{
			start_idx = *(not_processed_vertices_set.begin());
		}
		connex_components.push_back(ComputeConnexComponent(not_processed_vertices_set, start_idx));
	}
	return connex_components;
}

bool NeighborGraph::CheckSolutionConnexity(const Solution& solution) const
{
	unordered_set<int> solution_vertices_set = solution.get_solution_set();
	// Computing the connex component starting at the well to see if the solution is connex
	vector<int> connex_component = ComputeConnexComponent(solution_vertices_set, 0);

	// Check to see if the well is included in the solution or not because it is always included in the connex component
	return (connex_component.size() == solution.get_solution_size()+1-solution.get_solution_set().count(0));
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
	return final_neighbors_set;
}

vector<int> const & NeighborGraph::GetNeighbors(int const & vertex_index) const
{
	return edges_vector.at(vertex_index);
}

bool NeighborGraph::CheckSolutionDomination(const Solution& solution) const
{
	return (solution.get_non_dominated_vertices_set().size() == 0);
}