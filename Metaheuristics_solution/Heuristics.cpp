#include "Heuristics.h"

Solution NaiveHeuristic(NeighborGraph captation_graph, const NeighborGraph& communication_graph)
{
	// The captation graph is copied so it can be altered afterwards
	Solution heuristic_solution(communication_graph.get_vertices_number(), false);
	// Going through the degree vector to see which vertex has the highest degree
	int highest_degree_vertex_idx = 0;
	int highest_degree = 0;
	std::unordered_map<int, int> const & captation_degree_hashtable = captation_graph.get_degree_hashtable();
	for (int vertex_idx = 0; vertex_idx < captation_graph.get_vertices_number(); vertex_idx++)
	{
		if (captation_degree_hashtable.at(vertex_idx) > highest_degree)
		{
			highest_degree = captation_degree_hashtable.at(vertex_idx);
			highest_degree_vertex_idx = vertex_idx;
		}
	}

	std::cout << "Found highest degree : " << highest_degree << std::endl;

	heuristic_solution.AddVertexToTheSolution(highest_degree_vertex_idx);
	captation_graph.RemoveVertexAndNeighbors(highest_degree_vertex_idx);

	return Solution();
}