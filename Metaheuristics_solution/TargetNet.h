#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "Target.h"

using namespace std;

// Class that represents a set of targets
class TargetNet
{
	// The index vector for a given target might be different from the target index
	vector<Target> target_vect;
	// Maps each target index to its index in the input file
	map<int, int> target_idx_map;
public:
	// Default constructor
	TargetNet();
	TargetNet(const string& file_name);
	// Default destructor
	virtual ~TargetNet();
	int get_target_number() const;
	vector<Target> const & get_target_vect() const;
};