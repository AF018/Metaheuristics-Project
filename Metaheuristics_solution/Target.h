#pragma once

// Class that represents a target
class Target
{
	int index;
	double x_coord;
	double y_coord;
public:
	Target();
	Target(const int& init_idx, const double& x_coord, const double& y_coord);
	virtual ~Target();
};

