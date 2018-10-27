#include "NeighborGraph.h"
#include "Solution.h"

Solution::Solution()
{
	solution_set = unordered_set<int>();
	solution_value = 0;
	solution_captation_graph = nullptr;
	solution_communication_graph = nullptr;
}

Solution::Solution(const int& vertex_number, NeighborGraph const * captation_graph, NeighborGraph const * communication_graph)
{
	solution_set = unordered_set<int>();
	graph_vertex_number = vertex_number;
	solution_captation_graph = captation_graph;
	solution_communication_graph = communication_graph;
}

Solution::~Solution()
{
}

unordered_set<int> const & Solution::get_solution_set() const
{
	return solution_set;
}

int Solution::get_solution_value(const bool& verbose) const
{
	bool domination_check = solution_captation_graph->CheckSolutionDomination(*this);
	bool connexity_check = solution_communication_graph->CheckSolutionConnexity(*this);
	cout << domination_check << " " << connexity_check << endl;
	return solution_value + !(domination_check && connexity_check) * 400;
}

int Solution::get_solution_size() const
{
	return solution_set.size();
}

bool Solution::IsVertexInSolution(int const & vertex_idx) const
{
	return (solution_set.count(vertex_idx) == 1);
}

void Solution::AddVertexToTheSolution(const int & vertex_idx)
{
	solution_set.insert(vertex_idx);
	solution_value++;
}

void Solution::AddVerticesToTheSolution(const vector<int>& vertex_indices)
{
	vector<int>::const_iterator it = vertex_indices.begin();
	for (; it != vertex_indices.end(); it++)
	{
		solution_set.insert(*it);
		solution_value++;
	}
}

bool Solution::SwapVertices(const int& leaving_idx, const int& incoming_idx)
{
	// Removes the vertex of index leaving_idx from the solution and adds incoming_idx
	// Returns false if incoming_idx as already in the solution
	solution_set.erase(leaving_idx);
	// Insertion attempt
	// The boolean tells if the incoming vertex was already counted or not
	bool incoming_vertex_not_in_solution_before = solution_set.insert(incoming_idx).second;
	if (not incoming_vertex_not_in_solution_before)
	{
		solution_value--;
	}
	return incoming_vertex_not_in_solution_before;
}
