
#include "Metaheuristics.h"

void read_parameter_file(string const & parameter_path, string& file_path, string& heuristic_result_path, string& SA_result_path,
	double& captation_radius, double& communication_radius,
	int& reconstruction_threshold, int& SA_iteration_number,
	double& init_temperature, double& decreasing_coef, double& final_temperature)
{
	std::ifstream file(parameter_path.c_str());
	string line;
	std::getline(file, line);
	// Path to the target network file
	file_path = line;
	std::getline(file, line);
	// Path to the result file for the heuristic
	heuristic_result_path = line;
	std::getline(file, line);
	// Path to the result file for the simulated annealing
	SA_result_path = line;
	std::getline(file, line);
	int start_pos = 0;
	int end_pos = line.find(' ', start_pos);
	// Radius for the captation graph
	captation_radius = std::stod(line.substr(start_pos, end_pos - start_pos));
	start_pos = end_pos + 1;
	end_pos = line.find(' ', start_pos);
	// Radius for the communication graph
	communication_radius = std::stod(line.substr(start_pos, end_pos - start_pos));
	std::getline(file, line);
	// Number of iterations with constraint tresspassing before calling the connexity reconstruction heuristic
	reconstruction_threshold = std::stoi(line);
	std::getline(file, line);
	// Number of times the simulated annealing process is called
	SA_iteration_number = std::stoi(line);
	std::getline(file, line);
	// Temperature at which the simulated annealing starts
	init_temperature = std::stod(line);
	std::getline(file, line);
	// Decreasing coeeficient for the simulated annealing
	decreasing_coef = std::stod(line);
	std::getline(file, line);
	// Temperature at which the simulated annealing stops
	final_temperature = std::stod(line);
}

void CheckAndAddToPotentialSolutions(const int & potential_vertex_idx, unordered_set<int>& potential_solution_vertices_set,
	const NeighborGraph & communication_graph, unordered_set<int> const & solution_set)
{
	bool has_a_solution_neighbor = false;
	vector<int>::const_iterator potential_vertex_neighbor_it = communication_graph.get_edges_vector()[potential_vertex_idx].begin();
	// Looks at all neighbors and stops when finding one that is included in the solution
	for (; (not has_a_solution_neighbor) && potential_vertex_neighbor_it != communication_graph.get_edges_vector()[potential_vertex_idx].end(); potential_vertex_neighbor_it++)
	{
		if (solution_set.count(*potential_vertex_neighbor_it) == 1)
		{
			has_a_solution_neighbor = true;
		}
	}
	if (has_a_solution_neighbor)
	{
		potential_solution_vertices_set.insert(potential_vertex_idx);
	}
}

void RemoveVertexFromNeighborPotentialCovering(const int & vertex_idx, const int & vertex_to_delete_idx, 
	unordered_map<int, vector<int> >& covering_potential_map, unordered_set<int>& potential_solution_vertices_set)
{
	// Finding the vertex to delete in the vertices that the vertex_idx could have dominated
	unordered_map<int, vector<int> >::iterator covering_potential_it = covering_potential_map.find(vertex_idx);
	if (covering_potential_it != covering_potential_map.end())
	{
		vector<int>::iterator vertex_to_delete_it = find(covering_potential_it->second.begin(),
			covering_potential_it->second.end(),
			vertex_to_delete_idx);
		covering_potential_it->second.erase(vertex_to_delete_it);
		if (covering_potential_it->second.size() == 0)
		{
			covering_potential_map.erase(covering_potential_it);
			potential_solution_vertices_set.erase(vertex_idx);
		}
	}
}

void SimulatedAnnealingSearch(Solution& current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph,
	const double& init_temperature, const double& final_temperature, const double& decreasing_coef, const int& reconstruction_threshold)
{
	double temperature = init_temperature;
	int reconstruction_counter = 0;

	srand(0);
	//srand(time(NULL));

	vector<vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	int current_value = current_solution.get_solution_value();;
	int previous_value = -1;
	double acceptance_probability = -1;
	// Variables to keep in memory the best solution encountered
	Solution best_solution = current_solution;
	int best_solution_value = current_value;

	while (temperature > final_temperature)
	{
		previous_value = current_value;
		int random_vertex_idx = rand() % captation_edges_vector.size();

		// Removing all the vertices that dominate random_vertex_idx, which may include itself
		vector<int> dominating_vertices_vector = current_solution.get_dominating_neighbors_vector()[random_vertex_idx];
		// Vector keeping the indices of the vertices that have been removed
		vector<int> removed_vertices_vector;
		for (auto dominating_vertex : dominating_vertices_vector)
		{
			current_solution.RemoveVertexFromSolution(dominating_vertex);
			removed_vertices_vector.push_back(dominating_vertex);
		}
		
		unordered_set<int> const & non_dominated_vertices_set = current_solution.get_non_dominated_vertices_set();
		unordered_set<int> const & solution_set = current_solution.get_solution_set();
		// Container including the vertices neighbors of non dominated vertex(ices) in the communication graph
		// and which also have neighbor(s) in the communication graph included in the current solution
		unordered_set<int> potential_solution_vertices_set = unordered_set<int>();
		for (auto non_dominated_vertex : non_dominated_vertices_set)
		{
			for (auto potential_vertex : captation_edges_vector[non_dominated_vertex])
			{
				CheckAndAddToPotentialSolutions(potential_vertex, potential_solution_vertices_set, communication_graph, solution_set);
			}
			CheckAndAddToPotentialSolutions(non_dominated_vertex, potential_solution_vertices_set, communication_graph, solution_set);
		}
		// Computing how many vertices each non dominated vertex can dominate
		unordered_map<int, vector<int> > covering_potential_map = unordered_map<int, vector<int> >();
		for (auto non_dominated_vertex : non_dominated_vertices_set)
		{
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
		vector<int> selected_vertices_vector;
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

			// Removing the added vertex and its neighbors in the covering potential map
			for (auto added_vertex_neighbor : captation_edges_vector[added_vertex_idx])
			{
				// The following removals are only valid if the added_vertex_neighbor was not dominated
				if (non_dominated_vertices_set.count(added_vertex_neighbor) == 1)
				{
					for (auto neighbor_of_neighbor : captation_edges_vector[added_vertex_neighbor])
					{
						// Removing the added_vertex_neighbor from the covering potential vector of its neighbors
						RemoveVertexFromNeighborPotentialCovering(neighbor_of_neighbor, added_vertex_neighbor,
							covering_potential_map, potential_solution_vertices_set);
					}
					// Removing the added_vertex_neighbor from its own covering potential vector
					RemoveVertexFromNeighborPotentialCovering(added_vertex_neighbor, added_vertex_neighbor,
						covering_potential_map, potential_solution_vertices_set);
				}
				if (non_dominated_vertices_set.count(added_vertex_idx) == 1)
				{
					// Removing the added_vertex_idx from its neighbor covering potential vector if it was not dominated
					RemoveVertexFromNeighborPotentialCovering(added_vertex_neighbor, added_vertex_idx,
						covering_potential_map, potential_solution_vertices_set);
				}
			}

			// Adding the new solution vertex to the solution
			current_solution.AddVertexToTheSolution(added_vertex_idx);
			selected_vertices_vector.push_back(added_vertex_idx);
			// The added_vertex_idx was already removed from the potential_solution_vertices_set when exploring the neighbors of neighbors
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
		if (not communication_graph.CheckSolutionConnexity(current_solution))
		{
			reconstruction_counter++;
			if (reconstruction_counter == reconstruction_threshold)
			{
				ConnexityReconstructionHeuristic(current_solution, communication_graph, selected_vertices_vector);
				reconstruction_counter = 0;
			}
		}
		current_value = current_solution.get_solution_value();
		//cout << "current value : " << current_value << " old value  " << previous_value << endl;
		if ((current_value > previous_value) && ((rand() / double(RAND_MAX)) > acceptance_probability))
		{
			acceptance_probability = exp((previous_value - current_value) / temperature);
			// Going back to the previous solution
			for (auto added_vertex : selected_vertices_vector)
			{
				current_solution.RemoveVertexFromSolution(added_vertex);
			}
			for (auto removed_vertex : removed_vertices_vector)
			{
				current_solution.AddVertexToTheSolution(removed_vertex);
			}
			current_value = previous_value;
			//cout << "                    Went back to the old value : " << current_solution.get_solution_value() << endl;
		}

		if (best_solution_value > current_solution.get_solution_value())
		{
			best_solution = current_solution;
			best_solution_value = current_value;
		}
		// Computation of the next iteration temperature
		temperature = decreasing_coef * temperature;
	}
	//communication_graph.CheckSolutionConnexity(current_solution);
	cout << "Final value : " << current_value << endl;
	cout << "Returning solution with value : " << best_solution_value << endl;
	current_solution = best_solution;
}

void LocalSearch(Solution current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph)
{
	int iteration_amount = 1000;
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
			DominationReconstructionHeuristic(current_solution, captation_graph);
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