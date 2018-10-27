
#include "Metaheuristics.h"

void TabooSearch(Solution current_solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph)
{
	int iteration_amount = 50;

	srand(0);
	//srand(time(NULL));

	vector<vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	int best_neighboring_solution_idx = -1;
	int best_neighboring_solution_value = 0;
	int neighboring_solution_value = 0;
	Solution best_solution = current_solution;
	int best_solution_value = current_solution.get_solution_value();

	for (int iteration_counter=0; iteration_counter < iteration_amount; iteration_counter++)
	{
		int random_idx = rand() % current_solution.get_solution_size();
		unordered_set<int>::iterator const solution_set_it = std::next(current_solution.get_solution_set().begin(), random_idx);
		cout << "random index : " << *solution_set_it << " current value : " << current_solution.get_solution_value() << endl;

		// Initializing the best neighboring solution value to an upper bound
		best_neighboring_solution_value = 2 * captation_graph.get_vertices_number();
		for (auto neighbor_vector_it : captation_edges_vector[*solution_set_it])
		{
			bool valid_swap = current_solution.SwapVertices(random_idx, neighbor_vector_it);
			neighboring_solution_value = current_solution.get_solution_value(true);
			cout << "idx : " << neighbor_vector_it << " value : " << neighboring_solution_value << endl;
			if (best_neighboring_solution_value > neighboring_solution_value)
			{
				best_neighboring_solution_idx = neighbor_vector_it;
				best_neighboring_solution_value = neighboring_solution_value;
			}
			// Going back to test other neighboring solutions
			if (valid_swap)
			{
				// Do the opposite swap if the neighbor_vector_it vertex was not
				// included in the solution before the swap
				current_solution.SwapVertices(neighbor_vector_it, random_idx);
			}
			else
			{
				// Otherwise just add back the random_idx vertex
				current_solution.AddVertexToTheSolution(random_idx);
			}
			//cout << "return to value : " << current_solution.get_solution_value() << endl;
		}
		current_solution.SwapVertices(random_idx, best_neighboring_solution_idx);
		if (best_solution_value > best_neighboring_solution_value)
		{
			best_solution = current_solution;
			best_solution_value = best_neighboring_solution_value;
		}
		cout << "Chosen neighbor : " <<  best_neighboring_solution_idx << " value : " << current_solution.get_solution_value() << endl;
		cout << endl;
	}
}