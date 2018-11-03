#include "Metaheuristics.h"

#include <ctime>

int main()
{
	//srand(0);
	srand(time(NULL));

	string parameter_file_path = "C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/run_parameters.dat";
	string grid_path;
	string solution_path;
	string heuristic_result_path;
	string SA_result_path;
	double captation_radius;
	double communication_radius;
	int reconstruction_threshold;
	int simulated_annealing_iteration_number;
	double init_temperature;
	double decreasing_coef;
	double final_temperature;
	read_parameter_file(parameter_file_path, grid_path, solution_path,
		heuristic_result_path, SA_result_path,
		captation_radius, communication_radius,
		reconstruction_threshold, simulated_annealing_iteration_number,
		init_temperature, decreasing_coef, final_temperature);

	std::clock_t start;
	double duration;

	cout << "Reading file" << endl;
	start = std::clock();
	TargetNet target_net(grid_path);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Reading duration : " << duration << " seconds" << endl;;
	cout << endl;

	cout << "Constructing captation graph" << endl;
	start = std::clock();
	NeighborGraph captation_graph(target_net, captation_radius);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Captation graph construction duration : " << duration << " seconds" << endl;
	cout << endl;
	
	cout << "Constructing communication graph" << endl;
	start = std::clock();
	NeighborGraph communication_graph(target_net, communication_radius);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Communication graph construction duration : " << duration << " seconds" << endl;
	cout << endl;
	
	Solution current_solution;
	if (solution_path == "0")
	{
		cout << "Runnning the naive random heuristic" << endl;
		start = std::clock();
		current_solution = NaiveRandomHeuristic(captation_graph, communication_graph);
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		cout << "Naive random heuristic duration : " << duration << " seconds" << endl;
		cout << "Naive random heuristic score : " << current_solution.get_solution_value() << " used targets" << endl;
		current_solution.Write(heuristic_result_path);
		cout << endl;
	}
	else
	{
		cout << "Recovering the solution at " << solution_path << endl;
		current_solution = Solution(captation_graph.get_vertices_number(), &captation_graph, &communication_graph);
		current_solution.AddSolutionFile(solution_path);
		cout << endl;
	}

	cout << "Runnning the checks on the naive solution" << endl;
	start = std::clock();
	captation_graph.CheckSolutionDomination(current_solution);
	communication_graph.CheckSolutionConnexity(current_solution);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Check duration : " << duration << " seconds" << endl;
	cout << endl;

	cout << "Runnning the simulated annealing : " << simulated_annealing_iteration_number << " iteration(s)" << endl;
	start = std::clock();
	for (int iteration_number = 0; iteration_number < simulated_annealing_iteration_number; iteration_number++)
	{
		SimulatedAnnealingSearch(current_solution, captation_graph, communication_graph,
			init_temperature, final_temperature, decreasing_coef, reconstruction_threshold);
	}
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	cout << "Local search duration : " << duration << " seconds" << endl;
	cout << "Local search score : " << current_solution.get_solution_value() << " used targets" << endl;
	cout << endl;

	current_solution.Write(SA_result_path);

	return 0;
}