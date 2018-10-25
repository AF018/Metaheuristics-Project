#include "Heuristics.h"

#include <ctime>

int main()
{
	// Modify the path to access your favorite dat file
	//std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/captANOR225_9_20.dat");
	//std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/captANOR1500_21_500.dat");
	std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/test_1.dat");

	std::clock_t start;
	double duration;

	std::cout << "Reading File" << std::endl;
	start = std::clock();
	TargetNet target_net(file_path);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << std::endl;

	std::cout << "Constructing Captation Graph" << std::endl;
	start = std::clock();
	NeighborGraph captation_graph(target_net, 1.0);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << std::endl;
	
	std::cout << "Constructing Communication Graph" << std::endl;
	start = std::clock();
	NeighborGraph communication_graph(target_net, 2.0);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";
	std::cout << std::endl;
	
	std::cout << "Runnning the naive heuristic" << std::endl;
	start = std::clock();
	Solution naive_solution = NaiveHeuristic(captation_graph, communication_graph);
	captation_graph.CheckSolutionDomination(naive_solution);
	communication_graph.CheckSolutionConnexity(naive_solution);
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";

	return 0;
}