
#include "Metaheuristics.h"

void TabuSearch(Solution& current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph)
{
	int iteration_amount = 2;

	srand(0);
	//srand(time(NULL));

	vector<vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	// Variables to keep in memory the best solution encountered
	Solution best_solution = current_solution;
	int best_solution_value = current_solution.get_solution_value();

	for (int iteration_counter = 0; iteration_counter < iteration_amount; iteration_counter++)
	{
		//cout << "size captation edeges vector : " << captation_edges_vector.size() << endl;
		int random_vertex_idx = rand() % captation_edges_vector.size();
		cout << "random index : " << random_vertex_idx << " current value : " << current_solution.get_solution_value() << endl;

		// Removing all the vertices that dominate random_vertex_idx, which may include itself
		vector<int> dominating_vertices_vector = current_solution.get_dominating_neighbors_vector()[random_vertex_idx];
		for (auto dominating_vertex_it : dominating_vertices_vector)
		{
			current_solution.RemoveVertexFromSolution(dominating_vertex_it);
			cout << "removing : " << dominating_vertex_it << " size : " << current_solution.get_solution_size() << endl;
		}
		
		//cout << "formation ensemble des sommets potentiels" << endl;
		unordered_set<int> const & non_dominated_vertices_set = current_solution.get_non_dominated_vertices_set();
		unordered_set<int> const & solution_set = current_solution.get_solution_set();
		// Container including the vertices neighbors of non dominated vertex(ices) in the communication graph
		// and which also have neighbor(s) in the communication graph included in the current solution
		unordered_set<int> potential_solution_vertices_set = unordered_set<int>();
		for (auto non_dominated_vertex : non_dominated_vertices_set)
		{
			for (auto potential_vertex : captation_edges_vector[non_dominated_vertex])
			{
				bool has_a_solution_neighbor = false;
				for (auto non_dominated_vertex_neighbor : communication_graph.get_edges_vector()[potential_vertex])
				{
					if (solution_set.count(non_dominated_vertex_neighbor) == 1)
					{
						has_a_solution_neighbor = true;
					}
				}
				if (has_a_solution_neighbor)
				{
					potential_solution_vertices_set.insert(potential_vertex);
				}
			}
			// For each non dominated_vertex, see if it has a neighbor (in the communication graph) that
			// is included in the solution
			bool has_a_solution_neighbor = false;
			for (auto non_dominated_vertex_neighbor : communication_graph.get_edges_vector()[non_dominated_vertex])
			{
				if (solution_set.count(non_dominated_vertex_neighbor) == 1)
				{
					has_a_solution_neighbor = true;
				}
			}
			if (has_a_solution_neighbor)
			{
				cout << non_dominated_vertex << endl;
				potential_solution_vertices_set.insert(non_dominated_vertex);
			}
		}
		// Computing how many vertices each non dominated vertex can dominate
		unordered_map<int, vector<int> > covering_potential_map = unordered_map<int, vector<int> >();
		for (auto non_dominated_vertex : non_dominated_vertices_set)
		{
			cout << "non dominated : " << non_dominated_vertex << endl;
			covering_potential_map[non_dominated_vertex] = vector<int>();
			covering_potential_map[non_dominated_vertex].push_back(non_dominated_vertex);
			for (auto non_dominated_vertex_neighbor : captation_edges_vector[non_dominated_vertex])
			{
				if (non_dominated_vertices_set.count(non_dominated_vertex_neighbor) == 1)
				{
					// Adding to the covering potential of non_dominated_vertex if the vertex is not dominated
					covering_potential_map[non_dominated_vertex].push_back(non_dominated_vertex_neighbor);
				}
				else
				{
					// Else adding to the covering potential map of the dominated neighbor
					// This way all the covering potentials of the dominated vertices are filled at the end
					if (covering_potential_map.find(non_dominated_vertex_neighbor) == covering_potential_map.end())
					{
						covering_potential_map[non_dominated_vertex_neighbor] = vector<int>();
					}
					covering_potential_map[non_dominated_vertex_neighbor].push_back(non_dominated_vertex);
				}
			}
		}
		int highest_covering_potential = 0;
		// Vector including the vertices with highest covering potential
		vector<int> potential_new_solution_vector;
		// Vector including all the selected vertices for the solution
		vector<int> selected_vertices;
		while (non_dominated_vertices_set.size() > 0)
		{
			highest_covering_potential = 0;
			potential_new_solution_vector = vector<int>();
			for (auto potential_solution_idx : potential_solution_vertices_set)
			{
				if (highest_covering_potential < covering_potential_map[potential_solution_idx].size())
				{
					highest_covering_potential = covering_potential_map[potential_solution_idx].size();
					potential_new_solution_vector = vector<int>();
					potential_new_solution_vector.push_back(potential_solution_idx);
				}
				else if (highest_covering_potential == covering_potential_map[potential_solution_idx].size())
				{
					highest_covering_potential = covering_potential_map[potential_solution_idx].size();
					potential_new_solution_vector.push_back(potential_solution_idx);
				}
			}

			int random_idx = rand() % potential_new_solution_vector.size();
			int added_vertex_idx = potential_new_solution_vector[random_idx];
			selected_vertices.push_back(added_vertex_idx);

			cout << "added_vertex_idx : " << added_vertex_idx << " potential " << highest_covering_potential << endl;

			// Removing the added vertex and its neighbors in the covering potential map
			for (auto added_vertex_neighbor : captation_edges_vector[added_vertex_idx])
			{
				// Removing the neighbor vertex from the containers keeping in memory the potential cover of each vertex
				// if this neighbor vertex was a non dominated one
				if (non_dominated_vertices_set.count(added_vertex_neighbor) == 1)
				{
					for (auto neighbor_of_neighbor : captation_edges_vector[added_vertex_neighbor])
					{
						// Checking if the vertex is non dominated
						unordered_map<int, vector<int> >::iterator neighbor_of_neighbor_it = covering_potential_map.find(neighbor_of_neighbor);
						if (neighbor_of_neighbor_it != covering_potential_map.end())
						{
							vector<int>::iterator vertex_to_delete_it = find(neighbor_of_neighbor_it->second.begin(),
								neighbor_of_neighbor_it->second.end(),
								added_vertex_neighbor);
							neighbor_of_neighbor_it->second.erase(vertex_to_delete_it);
							if (neighbor_of_neighbor_it->second.size() == 0)
							{
								potential_solution_vertices_set.erase(neighbor_of_neighbor_it->first);
								covering_potential_map.erase(neighbor_of_neighbor_it);
							}
						}
					}
				}
				if (non_dominated_vertices_set.count(added_vertex_idx) == 1)
				{
					unordered_map<int, vector<int> >::iterator neighbor_it = covering_potential_map.find(added_vertex_neighbor);
					if (neighbor_it != covering_potential_map.end())
					{
						vector<int>::iterator vertex_to_delete_it = find(neighbor_it->second.begin(),
							neighbor_it->second.end(),
							added_vertex_idx);
						neighbor_it->second.erase(vertex_to_delete_it);

						// If the neighbor cannot dominate any vertex if added, it is erased
						if (neighbor_it->second.size() == 0)
						{
							potential_solution_vertices_set.erase(neighbor_it->first);
							covering_potential_map.erase(neighbor_it);
						}
					}
				}
			}

			// Adding the new solution vertex to the solution
			current_solution.AddVertexToTheSolution(added_vertex_idx);
			potential_solution_vertices_set.erase(added_vertex_idx);
			for (auto added_vertex_neighbor : communication_graph.get_edges_vector()[added_vertex_idx])
			{
				// Add the neighbors of the added vertex if they can dominate at least one non dominated vertex
				unordered_map<int, vector<int> >::iterator added_vertex_neighbor_it = covering_potential_map.find(added_vertex_neighbor);
				if (added_vertex_neighbor_it != covering_potential_map.end()
					&& added_vertex_neighbor_it->second.size() != 0)
				{
					potential_solution_vertices_set.insert(added_vertex_neighbor);
				}
			}
		}
		cout << "one iteration over" << endl;
		int current_value = current_solution.get_solution_value();
		if (best_solution_value > current_solution.get_solution_value())
		{
			best_solution = current_solution;
			best_solution_value = current_value;
		}
	}
	cout << "Final Tabu value : " << current_solution.get_solution_value() << endl;
	cout << "Returning solution with value : " << best_solution_value << endl;
	//current_solution = best_solution;
}

void TabuSearch_2(Solution current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph)
{
	int iteration_amount = 500;
	int reconstruction_bound = 15;

	srand(0);
	//srand(time(NULL));

	vector<vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	int best_neighboring_solution_idx = -1;
	int best_neighboring_solution_value = 0;
	int neighboring_solution_value = 0;
	// Booleans to remember the checks concerning connexity and domination
	bool best_neighbor_solution_domination;
	bool best_neighbor_solution_connexity;
	bool domination_condition;
	bool connexity_condition;
	// Variables to keep in memory the best solution encountered
	Solution best_solution = current_solution;
	int best_solution_value = current_solution.get_solution_value();
	// Variable for the reconstruction condition
	int reconstruction_counter = 0;

	for (int iteration_counter=0; iteration_counter < iteration_amount; iteration_counter++)
	{
		int random_number = rand() % current_solution.get_solution_size();
		int random_vertex_idx = *(std::next(current_solution.get_solution_set().begin(), random_number));
		cout << "random index : " << random_vertex_idx << " current value : " << current_solution.get_solution_value() << endl;

		// Initializing the best neighboring solution value to an upper bound
		best_neighboring_solution_value = UPPER_BOUND_VALUE;
		best_neighboring_solution_idx = -1;
		for (auto neighbor_vector_it : captation_edges_vector[random_vertex_idx])
		{
			// valid_swap is true if the neighbor vector was not included in the solution before
			bool valid_swap = current_solution.SwapVertices(random_vertex_idx, neighbor_vector_it);
			current_solution.get_solution_value(neighboring_solution_value, domination_condition, connexity_condition);
			//cout << "idx : " << neighbor_vector_it << " value : " << neighboring_solution_value << endl;
			if (best_neighboring_solution_value > neighboring_solution_value)
			{
				best_neighboring_solution_idx = neighbor_vector_it;
				best_neighboring_solution_value = neighboring_solution_value;
				best_neighbor_solution_domination = domination_condition;
				best_neighbor_solution_connexity = connexity_condition;
			}
			// Going back to test other neighboring solutions
			if (valid_swap)
			{
				current_solution.SwapVertices(neighbor_vector_it, random_vertex_idx);
			}
			else
			{
				current_solution.AddVertexToTheSolution(random_vertex_idx);
			}
			//cout << "return to value : " << current_solution.get_solution_value() << endl;
		}
		//if (not best_neighbor_solution_domination || not best_neighbor_solution_connexity)
		//{
		//	//cout << "emergency neighborhood" << endl;
		//	current_solution.RemoveVertexFromSolution(random_vertex_idx);
		//	for (auto neighbor_vector_it : captation_edges_vector[random_vertex_idx])
		//	{
		//		current_solution.AddVertexToTheSolution(neighbor_vector_it);
		//	}
		//}
		//else
		//{
			current_solution.SwapVertices(random_vertex_idx, best_neighboring_solution_idx);
			if (not best_neighbor_solution_connexity || not best_neighbor_solution_domination)
			{
				reconstruction_counter++;
			}
		//}
		if (reconstruction_counter > reconstruction_bound)
		{
			ReconstructionHeuristic(current_solution, captation_graph, communication_graph);
		}
		if (best_solution_value > best_neighboring_solution_value)
		{
			best_solution = current_solution;
			best_solution_value = best_neighboring_solution_value;
		}
		//cout << "Chosen neighbor : " <<  best_neighboring_solution_idx << " value : " << current_solution.get_solution_value() << endl;
		//cout << endl;
	}
	cout << "Final Tabu value : " << current_solution.get_solution_value() << endl;
	cout << "Returning solution with value : " << best_solution_value << endl;
	current_solution = best_solution;
}