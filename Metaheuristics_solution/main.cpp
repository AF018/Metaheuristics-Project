#include "Targetnet.h"

int main()
{
	std::cout << "Hello me" << std::endl;

	// Modify the path to access your favorite dat file
	std::string file_path("C:/Users/Antoine/Documents/Ecole des Ponts/Cours 3A/Metaheuristiques/Metaheuristics-project/Instances/captANOR225_9_20.dat");
	
	TargetNet target_net(file_path);

	return 0;
}