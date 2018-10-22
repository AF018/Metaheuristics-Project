#pragma once

#include <vector>

// Class encoding a solution for the optimization problem
class Solution
{
	// Vector of size equal to the number of vertices
	// Each cell i tells if the i-th vertex is included in the solution
	std::vector<bool> solution_vect;
	int solution_value;
public:
	// Default constructor
	Solution();
	// Creates a solution with all the elements if put_all_vertices==true, and none otherwise
	Solution(const int& vertex_number, const bool& put_all_vertices);
	// Default destructor
	virtual ~Solution();
	std::vector<bool> const * get_solution_vect() const;
	bool IsVertexInSolution(int const& vertex_idx) const;
	void AddVertexToTheSolution(const int& vertex_idx);
	void AddVerticesToTheSolution(const std::vector<int>& vertex_indices);
};

