#pragma once

// Class that represents a target
// Two indices are used, the first one only corresponds 
class Target
{
	int index;
	double x_coord;
	double y_coord;
public:
	Target();
	Target(const int& init_idx, const double& x_coord, const double& y_coord);
	virtual ~Target();
	int get_index() const;
	double get_x_coord() const;
	double get_y_coord() const;
};

