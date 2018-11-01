#include "Heuristics.h"

Solution NaiveRandomHeuristic(const NeighborGraph& captation_graph, const NeighborGraph& communication_graph)
{
	srand(0);
	//srand(time(NULL));

	vector<int> solution_vertices_vector;
	set<int> covered_vertices_set;
	vector<vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	// Vector keeping in memory the vertices it can cover if added to the solution
	vector<unordered_set<int> > covering_potential_vector(captation_edges_vector.size());
	for (int vertex_idx = 0; vertex_idx < covering_potential_vector.size(); vertex_idx++)
	{
		covering_potential_vector[vertex_idx] = unordered_set<int>(captation_edges_vector[vertex_idx].begin(),
			captation_edges_vector[vertex_idx].end());
	}
	// First iteration
	vector<int> highest_potential_vertex_vector = vector<int>();
	int highest_potential = -1;
	int current_potential = -1;
	for (int current_vertex_idx = 0; current_vertex_idx < communication_graph.get_vertices_number(); current_vertex_idx++)
	{
		current_potential = captation_edges_vector[current_vertex_idx].size();
		if (current_potential == highest_potential)
		{
			highest_potential_vertex_vector.push_back(current_vertex_idx);
		}
		else if (current_potential > highest_potential)
		{
			highest_potential = current_potential;
			highest_potential_vertex_vector = vector<int>();
			highest_potential_vertex_vector.push_back(current_vertex_idx);
		}
	}
	// Choose one random vertex among the ones with highest potential
	int random_choice = rand() % highest_potential_vertex_vector.size();
	int random_choice_vertex = highest_potential_vertex_vector[random_choice];
	solution_vertices_vector.push_back(random_choice_vertex);
	vector<int> const & neighbors_idx_vector = captation_edges_vector.at(random_choice_vertex);
	covered_vertices_set.insert(random_choice_vertex);
	for (vector<int>::const_iterator neighbors_idx_vector_it = neighbors_idx_vector.begin(); neighbors_idx_vector_it != neighbors_idx_vector.end(); neighbors_idx_vector_it++)
	{
		covered_vertices_set.insert(*neighbors_idx_vector_it);
		for (auto neighbors_of_neighbor_it : captation_edges_vector[*neighbors_idx_vector_it])
		{
			covering_potential_vector[neighbors_of_neighbor_it].erase(*neighbors_idx_vector_it);
		}
		covering_potential_vector[*neighbors_idx_vector_it].erase(random_choice_vertex);
	}
	// Iterations after the first one
	while (covered_vertices_set.size() < communication_graph.get_vertices_number())
	{
		set<int> const & neighbors_set = communication_graph.GetNeighbors(solution_vertices_vector);;
		// Going through the neighbors to see which vertex has the highest number of not covered neighbors
		set<int>::iterator neighbors_set_it = neighbors_set.begin();
		highest_potential_vertex_vector = vector<int>();
		highest_potential = -1;
		current_potential = -1;
		// Computing the vertex index that can cover a maximal number of uncovered vertices;
		for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			current_potential = covering_potential_vector[*neighbors_set_it].size();
			if (current_potential == highest_potential)
			{
				highest_potential_vertex_vector.push_back(*neighbors_set_it);
			}
			else if (current_potential > highest_potential)
			{
				highest_potential = current_potential;
				highest_potential_vertex_vector = vector<int>();
				highest_potential_vertex_vector.push_back(*neighbors_set_it);
			}
		}
		random_choice = rand() % highest_potential_vertex_vector.size();
		random_choice_vertex = highest_potential_vertex_vector[random_choice];
		solution_vertices_vector.push_back(random_choice_vertex);
		vector<int> const & neighbors_idx_vector = captation_edges_vector.at(random_choice_vertex);
		covered_vertices_set.insert(random_choice_vertex);
		vector<int>::const_iterator neighbors_idx_vector_it = neighbors_idx_vector.begin();
		for (; neighbors_idx_vector_it != neighbors_idx_vector.end(); neighbors_idx_vector_it++)
		{
			covered_vertices_set.insert(*neighbors_idx_vector_it);
			for (auto neighbors_of_neighbor_it : captation_edges_vector[*neighbors_idx_vector_it])
			{
				covering_potential_vector[neighbors_of_neighbor_it].erase(*neighbors_idx_vector_it);
			}
			covering_potential_vector[*neighbors_idx_vector_it].erase(random_choice_vertex);
		}
	}

	Solution heuristic_solution(communication_graph.get_vertices_number(), &captation_graph, &communication_graph);
	vector<int>::iterator solution_vertices_vector_it = solution_vertices_vector.begin();
	for (; solution_vertices_vector_it != solution_vertices_vector.end(); solution_vertices_vector_it++)
	{
		heuristic_solution.AddVertexToTheSolution(*solution_vertices_vector_it);
	}

	return heuristic_solution;
}

Solution NaiveHeuristic(const NeighborGraph& captation_graph, const NeighborGraph& communication_graph)
{
	vector<int> solution_vertices_vector;
	set<int> covered_vertices_set;
	vector<vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	// Vector keeping in memory the vertices it can cover if added to the solution
	vector<unordered_set<int> > covering_potential_vector(captation_edges_vector.size());
	for (int vertex_idx = 0; vertex_idx < covering_potential_vector.size(); vertex_idx++)
	{
		covering_potential_vector[vertex_idx] = unordered_set<int>(captation_edges_vector[vertex_idx].begin(),
																		captation_edges_vector[vertex_idx].end());
	}
	// First iteration
	int highest_potential_vertex_idx = 0;
	int highest_potential = -1;
	int current_potential = -1;
	for (int current_vertex_idx = 0; current_vertex_idx < communication_graph.get_vertices_number(); current_vertex_idx++)
	{
		current_potential = captation_edges_vector[current_vertex_idx].size();
		if (current_potential > highest_potential)
		{
			highest_potential = current_potential;
			highest_potential_vertex_idx = current_vertex_idx;
		}
	}
	solution_vertices_vector.push_back(highest_potential_vertex_idx);
	vector<int> const & neighbors_idx_vector = captation_edges_vector.at(highest_potential_vertex_idx);
	covered_vertices_set.insert(highest_potential_vertex_idx);
	for (vector<int>::const_iterator neighbors_idx_vector_it = neighbors_idx_vector.begin(); neighbors_idx_vector_it != neighbors_idx_vector.end(); neighbors_idx_vector_it++)
	{
		covered_vertices_set.insert(*neighbors_idx_vector_it);
		for (auto neighbors_of_neighbor_it : captation_edges_vector[*neighbors_idx_vector_it])
		{
			covering_potential_vector[neighbors_of_neighbor_it].erase(*neighbors_idx_vector_it);
		}
		covering_potential_vector[*neighbors_idx_vector_it].erase(highest_potential_vertex_idx);
	}
	// Iterations after the first one
	while (covered_vertices_set.size() < communication_graph.get_vertices_number())
	{
		set<int> const & neighbors_set = communication_graph.GetNeighbors(solution_vertices_vector);;
		// Going through the neighbors to see which vertex has the highest number of not covered neighbors
		set<int>::iterator neighbors_set_it = neighbors_set.begin();
		highest_potential_vertex_idx = 0;
		highest_potential = -1;
		current_potential = -1;
		// Computing the vertex index that can cover a maximal number of uncovered vertices;
		for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			current_potential = covering_potential_vector[*neighbors_set_it].size();
			if (current_potential > highest_potential)
			{
				highest_potential = current_potential;
				highest_potential_vertex_idx = *neighbors_set_it;
			}
		}
		solution_vertices_vector.push_back(highest_potential_vertex_idx);
		vector<int> const & neighbors_idx_vector = captation_edges_vector.at(highest_potential_vertex_idx);
		covered_vertices_set.insert(highest_potential_vertex_idx);
		vector<int>::const_iterator neighbors_idx_vector_it = neighbors_idx_vector.begin();
		for (; neighbors_idx_vector_it != neighbors_idx_vector.end(); neighbors_idx_vector_it++)
		{
			covered_vertices_set.insert(*neighbors_idx_vector_it);
			for (auto neighbors_of_neighbor_it : captation_edges_vector[*neighbors_idx_vector_it])
			{
				covering_potential_vector[neighbors_of_neighbor_it].erase(*neighbors_idx_vector_it);
			}
			covering_potential_vector[*neighbors_idx_vector_it].erase(highest_potential_vertex_idx);
		}
	}

	Solution heuristic_solution(communication_graph.get_vertices_number(), &captation_graph, &communication_graph);
	vector<int>::iterator solution_vertices_vector_it = solution_vertices_vector.begin();
	for (; solution_vertices_vector_it != solution_vertices_vector.end(); solution_vertices_vector_it++)
	{
		cout << " | " << *solution_vertices_vector_it;
		heuristic_solution.AddVertexToTheSolution(*solution_vertices_vector_it);
	}
	cout << endl;

	return heuristic_solution;
}

void DominationReconstructionHeuristic(Solution& current_solution, const NeighborGraph& captation_graph)
{
	std::vector<vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	while (current_solution.get_non_dominated_vertices_set().size() != 0)
	{
		int non_covered_idx = *(current_solution.get_non_dominated_vertices_set().begin());
		std::vector<int> const & neighbors_vector = captation_edges_vector[non_covered_idx];
		int vertex_to_add_idx = neighbors_vector[rand()%neighbors_vector.size()];
		current_solution.AddVertexToTheSolution(vertex_to_add_idx);
	}
}

void ConnexityReconstructionHeuristic(Solution & current_solution, const NeighborGraph & communication_graph, vector<int> & selected_vertices_vector)
{
	vector<vector<int> > connex_components = communication_graph.ComputeConnexComponents(current_solution);
	while (connex_components.size() > 1)
	{
		int min_size = current_solution.get_solution_size();
		int min_size_component = -1;
		for (int component_idx = 0; component_idx != connex_components.size(); component_idx++)
		{
			if (connex_components[component_idx].size() < min_size)
			{
				min_size = connex_components[component_idx].size();
				min_size_component = component_idx;
			}
		}
		vector<vector<int> >::iterator min_connex_component_it = std::next(connex_components.begin(), min_size_component);
		std::random_shuffle(min_connex_component_it->begin(), min_connex_component_it->end());
		queue<int> bfs_queue;
		// Container mapping a vertex to its predecessor in the BFS
		// The elements from the minimal connex component are mapped to themselves
		unordered_map<int, int> already_visited_map;
		for (auto component_vertex : *min_connex_component_it)
		{
			already_visited_map[component_vertex] = component_vertex;
			for (auto component_neighbor : communication_graph.get_edges_vector()[component_vertex])
			{
				bfs_queue.push(component_neighbor);
				if (already_visited_map.find(component_neighbor) == already_visited_map.end())
				{
					already_visited_map[component_neighbor] = component_vertex;
				}
			}
		}
		int explored_vertex = -1;
		// While loop stops when a vertex in the solution and not in the minimal connex component is found
		do
		{
			explored_vertex = bfs_queue.front();
			bfs_queue.pop();
			for (auto explored_vertex_neighbor : communication_graph.get_edges_vector()[explored_vertex])
			{
				if (already_visited_map.find(explored_vertex_neighbor) == already_visited_map.end())
				{
					bfs_queue.push(explored_vertex_neighbor);
					already_visited_map[explored_vertex_neighbor] = explored_vertex;
				}
			}
		} while ((not current_solution.IsVertexInSolution(explored_vertex)) || (already_visited_map[explored_vertex] == explored_vertex));
		// Finding the connex component the algorithm just reached by going through each of them
		bool found_connected_component = false;
		int connected_component_idx = -1;
		for (int connex_component_idx = 0; connex_component_idx < connex_components.size() && not found_connected_component; connex_component_idx++)
		{
			vector<int>::iterator component_vertex_it = connex_components[connex_component_idx].begin();
			for (; component_vertex_it != connex_components[connex_component_idx].end() && not found_connected_component; component_vertex_it++)
			{
				if ((*component_vertex_it) == explored_vertex)
				{
					found_connected_component = true;
					connected_component_idx = connex_component_idx;
				}
			}
		}
		int path_vertex = already_visited_map[explored_vertex];
		while (already_visited_map[path_vertex] != path_vertex)
		{
			// Adding to the solution the elements of the path that form a connection between the two components
			connex_components[connected_component_idx].push_back(path_vertex);
			current_solution.AddVertexToTheSolution(path_vertex);
			selected_vertices_vector.push_back(path_vertex);
			path_vertex = already_visited_map[path_vertex];
		}

		for (auto min_connex_component_vertex : connex_components[min_size_component])
		{
			connex_components[connected_component_idx].push_back(min_connex_component_vertex);
		}

		if (min_size_component != connex_components.size() - 1)
		{
			// Moving the container just for complexity's sake
			// Otherwise all vectors are going to be shifted in the container when the erase is called
			connex_components[min_size_component] = connex_components[connex_components.size() - 1];
		}
		connex_components.erase(std::prev(connex_components.end()));
		int lol_size = 0;
		for (auto it : connex_components)
		{
			lol_size += it.size();
		}
		//cout << "total nb of elements in the connex components : " << lol_size << "  " << connex_components.size() << endl;
	}
}
