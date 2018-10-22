#include "NeighborGraph.h"

int main()
{
	std::cout << "Hello me" << std::endl;

	// Modify the path to access your favorite dat file
	//std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/captANOR225_9_20.dat");
	std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/test_1.dat");

	TargetNet target_net(file_path);
	NeighborGraph captation_graph(target_net, 1.0);
	NeighborGraph communication_graph(target_net, 2.0);

	// Check for the domination check function
	Solution naive_solution(target_net.get_target_number(), false);
	naive_solution.AddVertexToTheSolution(1);
	naive_solution.AddVertexToTheSolution(4);
	naive_solution.AddVertexToTheSolution(7);
	std::cout << captation_graph.CheckSolutionDominance(naive_solution) << std::endl;
	return 0;
}