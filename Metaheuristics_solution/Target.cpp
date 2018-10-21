#include "Target.h"

Target::Target()
{
	index = 0;
	double x_coord = 0;
	double y_coord = 0;
}

Target::Target(const int& init_idx, const double& init_x_coord, const double& init_y_coord)
{
	index = init_idx;
	x_coord = init_x_coord;
	y_coord = init_y_coord;
}

Target::~Target()
{
}

int Target::get_index() const
{
	return index;
}

double Target::get_x_coord() const
{
	return x_coord;
}

double Target::get_y_coord() const
{
	return y_coord;
}
