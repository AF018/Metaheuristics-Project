#include "Metaheuristics.h"

#include <ctime>

int main()
{
	// Modify the path to access your favorite dat file
	//std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/captANOR225_9_20.dat");
	std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/captANOR1500_21_500.dat");
	//std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/test_2.dat");

	double captation_radius = 1.0;
	double communication_radius = 1.0;

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
	
	std::cout << "Runnning the naive heuristic" << std::endl;
	start = std::clock();
	Solution naive_solution = NaiveHeuristic(captation_graph, communication_graph);
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

	TabooSearch(naive_solution, captation_graph, communication_graph);

	return 0;
}