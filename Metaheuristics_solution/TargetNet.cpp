#include "TargetNet.h"

TargetNet::TargetNet()
{
	target_map = std::map<int, Target>();
}

TargetNet::TargetNet(const std::string& file_name)
{
	target_map = std::map<int, Target>();

	// Variables to read the line and find the substrings
	std::ifstream file(file_name.c_str());
	std::string line;
	int start_pos = 0;
	int end_pos = 0;
	// Variables for the creation of each Target object
	int target_idx = 0;
	double target_x_coord = 0;
	double target_y_coord = 0;
	if (not file.is_open())
	{
		std::cout << "The file " << file_name << " is not found, please check it exists" << std::endl;
	}
	while (std::getline(file, line))
	{
		// Looking for the target index
		start_pos = 1;
		end_pos = line.find('  ', start_pos);
		target_idx = std::stoi(line.substr(start_pos, end_pos - start_pos));
		// Looking for the target x_coord
		// The search for the first coordinate starts after the "  " pattern that's been found
		start_pos = end_pos + 2;
		end_pos = line.find(' ', start_pos);
		target_x_coord = std::stod(line.substr(start_pos, end_pos - start_pos));
		// Looking for the target y_coord
		// Again, we increment the value so that the search does not stop at the " " pattern just found
		start_pos = end_pos + 1;
		end_pos = line.find(' ', start_pos);
		target_y_coord = std::stod(line.substr(start_pos, end_pos - start_pos));

		std::cout << target_idx << "  " << target_x_coord << "   " << target_y_coord << std::endl;
		target_map[target_idx] = Target(target_idx, target_x_coord, target_y_coord);
	}
	file.close();
	std::cout << "size : " << target_map.size() << std::endl;
}

TargetNet::~TargetNet()
{
}