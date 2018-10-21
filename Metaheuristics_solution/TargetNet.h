#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "Target.h"

// Class that represents a set of targets
class TargetNet
{
	std::map<int, Target> target_map;
public:
	TargetNet();
	TargetNet(const std::string& file_name);
	virtual ~TargetNet();
};