#include "Heuristics.h"

Solution NaiveHeuristic(NeighborGraph captation_graph, const NeighborGraph& communication_graph)
{
	// The captation graph is copied so it can be altered afterwards
	std::set<int> solution_vertices_set;
	std::set<int> copy_set;
	while (copy_set.size() < communication_graph.get_vertices_number())
	{
		std::cout << "starting over" << std::endl;
		std::set<int> neighbors_set;
		if (solution_vertices_set.size() == 0)
		{
			// First iteration, going through all the vertices
			for (int idx = 0; idx != captation_graph.get_vertices_number(); idx++)
			{
				neighbors_set.insert(idx);
			}
		}
		else
		{
			// Not the first iteration
			std::cout << "getting neighbors : " << solution_vertices_set.size() << std::endl;
			neighbors_set = communication_graph.GetNeighbors(solution_vertices_set);
			std::cout << "done with that : " << neighbors_set.size() << std::endl;
			for (std::set<int>::iterator it = neighbors_set.begin(); it != neighbors_set.end(); it++)
			{
				std::cout << *it << "  ";
			}
			std::cout << std::endl;
		}
		// Going through the degree hashtable to see which neighbor vertex has the highest degree
		std::unordered_map<int, NeighborGraphVertex *> const & captation_vertices_hashtable = captation_graph.get_vertices_hashtable();
		std::set<int>::iterator neighbors_set_it = neighbors_set.begin();
		int highest_degree_vertex_idx = 0;
		int highest_degree = -1;
		int current_degree = -1;
		for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			if ((*neighbors_set_it) == 3)
			{
				for (auto it : (captation_vertices_hashtable.at(*neighbors_set_it)->get_neighbors_set()))
				{
					std::cout << it->get_index() << std::endl;
				}
			}
			//std::cout << "        " << *neighbors_set_it;
			current_degree = captation_vertices_hashtable.at(*neighbors_set_it)->get_degree();
			if (current_degree > highest_degree)
			{
				highest_degree = current_degree;
				highest_degree_vertex_idx = *neighbors_set_it;
			}
		}
		std::cout << std::endl;
		std::cout << "highest degree vertex : " << highest_degree_vertex_idx << "degree : " << highest_degree << std::endl;
		solution_vertices_set.insert(highest_degree_vertex_idx);
		std::set<int> neighbors_idx_set = captation_graph.HeuristicRemoval(highest_degree_vertex_idx);
		copy_set.insert(highest_degree_vertex_idx);
		for (std::set<int>::iterator neighbors_idx_set_it = neighbors_idx_set.begin(); neighbors_idx_set_it != neighbors_idx_set.end(); neighbors_idx_set_it++)
		{
			std::cout << "  oui      " << *neighbors_idx_set_it;
			copy_set.insert(*neighbors_idx_set_it);
		}
		std::cout << std::endl;
	}

	Solution heuristic_solution(communication_graph.get_vertices_number(), false);
	std::set<int>::iterator solution_vertices_set_it = solution_vertices_set.begin();
	for (; solution_vertices_set_it != solution_vertices_set.end(); solution_vertices_set_it++)
	{
		std::cout << " | " << *solution_vertices_set_it;
		heuristic_solution.AddVertexToTheSolution(*solution_vertices_set_it);
	}
	std::cout << std::endl;

	return heuristic_solution;
}