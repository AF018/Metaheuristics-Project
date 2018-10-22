#include "Solution.h"

Solution::Solution()
{
	solution_vect = std::vector<bool>();
	solution_value = 0;
}

Solution::Solution(const int& vertex_number, const bool& put_all_vertices)
{
	solution_vect = std::vector<bool>(vertex_number, put_all_vertices);
	if (put_all_vertices)
	{
		solution_value = vertex_number;
	}
	else
	{
		solution_value = 0;
	}
}

Solution::~Solution()
{
}

std::vector<bool> const * Solution::get_solution_vect() const
{
	return &solution_vect;
}

bool Solution::IsVertexInSolution(int const & vertex_idx) const
{
	return solution_vect[vertex_idx];
}

void Solution::AddVertexToTheSolution(const int & vertex_idx)
{
	// Modify only if the vertex is not included yet
	if (not solution_vect[vertex_idx])
	{
		solution_vect[vertex_idx] = true;
		solution_value++;
	}
	
}

void Solution::AddVerticesToTheSolution(const std::vector<int>& vertex_indices)
{
	std::vector<int>::const_iterator it = vertex_indices.begin();
	for (; it != vertex_indices.end(); it++)
	{
		if (not solution_vect[*it])
		{
			solution_vect[*it] = true;
			solution_value++;
		}
	}
}

