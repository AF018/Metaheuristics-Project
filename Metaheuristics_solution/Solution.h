#pragma once

#include <unordered_set>

using namespace std;

class NeighborGraph;

// Class encoding a solution for the optimization problem
class Solution
{
	// Set containing all the vertices indices included in the solution 
	unordered_set<int> solution_set;
	// Container indicating for each vertex which neighbors dominate it
	vector<vector<int> > dominating_neighbors_vector;
	// Set containing all the vertices that are not dominated
	unordered_set<int> non_dominated_vertices_set;
	NeighborGraph const * solution_captation_graph;
	NeighborGraph const * solution_communication_graph;
public:
	// Default constructor
	Solution();
	// Creates a solution with no elements
	Solution(const int& vertex_number, NeighborGraph const * captation_graph, NeighborGraph const * communication_graph);
	// Default destructor
	virtual ~Solution();
	// Accessors
	unordered_set<int> const & get_solution_set() const;
	vector<vector<int> > const & get_dominating_neighbors_vector() const;
	unordered_set<int> const & get_non_dominated_vertices_set() const;
	int get_solution_value() const;
	void get_solution_value(int & solution_value, bool & domination_condition, bool & connexity_condition) const;
	int get_solution_size() const;
	// Checks if the vertex is included in solution_set
	bool IsVertexInSolution(int const& vertex_idx) const;
	// Returns true if the vertex was not inserted yet
	bool AddVertexToTheSolution(const int& vertex_idx);
	// Returns true if the vertex was indeed included in the solution before being removed 
	bool RemoveVertexFromSolution(const int & vertex_idx);
	// Operation for the metaheuristics, swaps two vertices
	// Returns true if the incoming vertex was not yet included in the solution
	bool SwapVertices(const int& leaving_idx, const int& incoming_idx);
	// Method writing in file_name the vertices in the solution, separated by a space
	void Write(const string& file_name);
	void AddSolutionFile(const string& file_name);
};

