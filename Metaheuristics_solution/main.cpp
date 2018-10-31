#include "Metaheuristics.h"

#include <ctime>

int main()
{
	// Modify the path to access your favorite dat file
	std::string project_dir_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/");
	std::string file_path(project_dir_path + "Instances/grid_15_15.dat");
	//std::string file_path(project_dir_path + "Instances/captANOR1500_21_500.dat");
	//std::string file_path(project_dir_path + "Instances/test_2.dat");

	double captation_radius = 1.0;
	double communication_radius = 2.0;

	std::clock_t start;
	double duration;

	std::cout << "Reading file" << std::endl;
	start = std::clock();
	TargetNet target_net(file_path);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << std::endl;

	std::cout << "Constructing captation graph" << std::endl;
	start = std::clock();
	NeighborGraph captation_graph(target_net, captation_radius);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << std::endl;
	
	std::cout << "Constructing communication graph" << std::endl;
	start = std::clock();
	NeighborGraph communication_graph(target_net, communication_radius);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << std::endl;
	
	std::cout << "Runnning the naive random heuristic" << std::endl;
	start = std::clock();
	Solution naive_solution = NaiveRandomHeuristic(captation_graph, communication_graph);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << "Score : " << naive_solution.get_solution_value() << " used targets \n";
	std::cout << std::endl;

	std::cout << "Runnning the checks on the naive solution" << std::endl;
	start = std::clock();
	captation_graph.CheckSolutionDomination(naive_solution, true);
	communication_graph.CheckSolutionConnexity(naive_solution, true);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << std::endl;

	naive_solution.Write(project_dir_path + "Results/naive_random_heuristic.dat");

	std::cout << "Runnning the tabu search" << std::endl;
	start = std::clock();
	SimulatedAnnealingSearch(naive_solution, captation_graph, communication_graph);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << std::endl;

	std::cout << "Local search score : " << naive_solution.get_solution_value() << " used targets \n";
	naive_solution.Write(project_dir_path + "Results/local_search_solution.dat");

	return 0;
}