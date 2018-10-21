#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Target.h"

// Class that represents a set of targets
class TargetNet
{
	// The index vector for a given target might be different from the target index
	std::vector<Target> target_vect;
	// Maps each target index to its index in the input file
	std::map<int, int> target_idx_map;
public:
	TargetNet();
	TargetNet(const std::string& file_name);
	virtual ~TargetNet();
	int get_target_number() const;
	const std::vector<Target>& get_target_vect() const;
};