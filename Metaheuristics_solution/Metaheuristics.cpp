
#include "Metaheuristics.h"

void TabuSearch(Solution current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph)
{
	int iteration_amount = 500;
	int reconstruction_bound = 3;

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
}