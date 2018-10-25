#include "Heuristics.h"

#include <ctime>

int main()
{
	std::cout << "Hello me" << std::endl;

	// Modify the path to access your favorite dat file
	//std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/captANOR225_9_20.dat");
	std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/captANOR1500_21_500.dat");

	std::clock_t start;
	double duration;

	TargetNet target_net(file_path);
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

	// Check for the domination check function
	//Solution naive_solution(target_net.get_target_number(), false);
	//naive_solution.AddVertexToTheSolution(1);
	//naive_solution.AddVertexToTheSolution(4);
	//naive_solution.AddVertexToTheSolution(8);
	//std::cout << captation_graph.CheckSolutionDomination(naive_solution) << std::endl;
	//std::cout << captation_graph.CheckSolutionConnexity(naive_solution) << std::endl;
	
	std::cout << "Runnning the naive heuristic" << std::endl;
	start = std::clock();

	Solution naive_solution = NaiveHeuristic(captation_graph, communication_graph);
	captation_graph.CheckSolutionDomination(naive_solution);
	communication_graph.CheckSolutionConnexity(naive_solution);

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds \n";

	
	return 0;
}