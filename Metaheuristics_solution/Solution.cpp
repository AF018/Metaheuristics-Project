#include "NeighborGraph.h"
#include "Solution.h"

Solution::Solution()
{
	solution_set = unordered_set<int>();
	non_dominated_vertices_set = unordered_set<int>();
	dominating_neighbors_vector = vector<vector<int> >();
	solution_captation_graph = nullptr;
	solution_communication_graph = nullptr;
}

Solution::Solution(const int& vertex_number, NeighborGraph const * captation_graph, NeighborGraph const * communication_graph)
{
	solution_set = unordered_set<int>();
	non_dominated_vertices_set = unordered_set<int>();
	// There is no vertex in the solution so no element is dominated
	for (int vertex_idx = 0; vertex_idx < captation_graph->get_vertices_number(); vertex_idx++)
	{
		non_dominated_vertices_set.insert(vertex_idx);
	}
	dominating_neighbors_vector = vector<vector<int> >(captation_graph->get_vertices_number());
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

unordered_set<int> const & Solution::get_non_dominated_vertices_set() const
{
	return non_dominated_vertices_set;
}

void Solution::get_solution_value(int & solution_value, bool & domination_condition, bool & connexity_condition) const
{
	domination_condition = (non_dominated_vertices_set.size() == 0);
	connexity_condition = solution_communication_graph->CheckSolutionConnexity(*this);
	solution_value = solution_set.size() + 1000 * !domination_condition + 10000 * !connexity_condition;
}

int Solution::get_solution_value() const
{
	bool domination_condition = (non_dominated_vertices_set.size() == 0);
	bool connexity_condition = solution_communication_graph->CheckSolutionConnexity(*this);
	return solution_set.size() + 1000 * !domination_condition + 10000 * !connexity_condition;
}

int Solution::get_solution_size() const
{
	return solution_set.size();
}

bool Solution::IsVertexInSolution(int const & vertex_idx) const
{
	return (solution_set.count(vertex_idx) == 1);
}

bool Solution::AddVertexToTheSolution(const int & vertex_idx)
{
	bool not_already_inserted = solution_set.insert(vertex_idx).second;
	if (not_already_inserted)
	{
		if (dominating_neighbors_vector[vertex_idx].size() == 0)
		{
			non_dominated_vertices_set.erase(vertex_idx);
		}
		dominating_neighbors_vector[vertex_idx].push_back(vertex_idx);
		for (auto vertex_neighbor_it : solution_captation_graph->get_edges_vector()[vertex_idx])
		{
			if (dominating_neighbors_vector[vertex_neighbor_it].size() == 0)
			{
				non_dominated_vertices_set.erase(vertex_neighbor_it);
			}
			dominating_neighbors_vector[vertex_neighbor_it].push_back(vertex_idx);
		}
	}
	return not_already_inserted;
}

bool Solution::RemoveVertexFromSolution(const int & vertex_idx)
{
	bool is_in_the_solution = (solution_set.count(vertex_idx) == 1);
	if (is_in_the_solution)
	{
		solution_set.erase(vertex_idx);
		vector<int>::iterator vertex_idx_it = std::find(dominating_neighbors_vector[vertex_idx].begin(),
			dominating_neighbors_vector[vertex_idx].end(),
			vertex_idx);
		dominating_neighbors_vector[vertex_idx].erase(vertex_idx_it);
		if (dominating_neighbors_vector[vertex_idx].size() == 0)
		{
			non_dominated_vertices_set.insert(vertex_idx);
		}
		for (auto vertex_neighbor_it : solution_captation_graph->get_edges_vector()[vertex_idx])
		{
			vertex_idx_it = std::find(dominating_neighbors_vector[vertex_neighbor_it].begin(),
				dominating_neighbors_vector[vertex_neighbor_it].end(),
				vertex_idx);
			dominating_neighbors_vector[vertex_neighbor_it].erase(vertex_idx_it);
			if (dominating_neighbors_vector[vertex_neighbor_it].size() == 0)
			{
				non_dominated_vertices_set.insert(vertex_neighbor_it);
			}
		}
	}
	return is_in_the_solution;
}

bool Solution::SwapVertices(const int& leaving_idx, const int& incoming_idx)
{
	// Removes the vertex of index leaving_idx from the solution and adds incoming_idx
	// Returns false if incoming_idx as already in the solution
	RemoveVertexFromSolution(leaving_idx);
	// Insertion attempt
	// The boolean tells if the incoming vertex was already counted or not
	bool incoming_vertex_not_already_in_solution = AddVertexToTheSolution(incoming_idx);
	return incoming_vertex_not_already_in_solution;
}

void Solution::Write(const string& file_name)
{
	std::ofstream file(file_name.c_str());

	for (auto it = solution_set.begin(); it != solution_set.end(); ++it) {
		file << *it << " ";
	}
	file << std::endl;

}