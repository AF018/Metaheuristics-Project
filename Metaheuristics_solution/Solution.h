#pragma once

#include <unordered_set>

using namespace std;

class NeighborGraph;

// Class encoding a solution for the optimization problem
class Solution
{
	// Vector of size equal to the number of vertices
	// Each cell i tells if the i-th vertex is included in the solution
	unordered_set<int> solution_set;
	int graph_vertex_number;
	int solution_value;
	NeighborGraph const * solution_captation_graph;
	NeighborGraph const * solution_communication_graph;
public:
	// Default constructor
	Solution();
	// Creates a solution with all the elements if put_all_vertices==true, and none otherwise
	Solution(const int& vertex_number, NeighborGraph const * captation_graph, NeighborGraph const * communication_graph);
	// Default destructor
	virtual ~Solution();
	unordered_set<int> const & get_solution_set() const;
	int get_solution_value(const bool& verbose = false) const;
	int get_solution_size() const;
	bool IsVertexInSolution(int const& vertex_idx) const;
	void AddVertexToTheSolution(const int& vertex_idx);
	void AddVerticesToTheSolution(const vector<int>& vertex_indices);
	// Operation for the metaheuristics, swaps the boolean value of two vertices
	bool SwapVertices(const int& leaving_idx, const int& incoming_idx);
};

