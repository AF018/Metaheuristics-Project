#include "Heuristics.h"

Solution NaiveHeuristic(NeighborGraph captation_graph, const NeighborGraph& communication_graph)
{
	std::unordered_map<int, std::set<int> > neighbor_indices_hashtable;
	for (int vertex_idx = 0; vertex_idx < captation_graph.get_vertices_number(); vertex_idx++)
	{
		neighbor_indices_hashtable[vertex_idx] = captation_graph.GetNeighbors(vertex_idx);
	}

	std::set<int> solution_vertices_set;
	std::set<int> covered_vertices_set;
	while (covered_vertices_set.size() < communication_graph.get_vertices_number())
	{
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
			//std::cout << "getting neighbors : " << solution_vertices_set.size() << std::endl;
			neighbors_set = communication_graph.GetNeighbors(solution_vertices_set);
			//std::cout << "done with that : " << neighbors_set.size() << std::endl;
			//for (std::set<int>::iterator it = neighbors_set.begin(); it != neighbors_set.end(); it++)
			//{
			//	std::cout << *it << "  ";
			//}
			//std::cout << std::endl;
		}
		// Going through the neighbors to see which vertex has the highest number of not covered neighbors
		std::unordered_map<int, NeighborGraphVertex *> const & captation_vertices_hashtable = captation_graph.get_vertices_hashtable();
		std::set<int>::iterator neighbors_set_it = neighbors_set.begin();
		int highest_degree_vertex_idx = 0;
		int highest_degree = -1;
		int current_degree = -1;
		for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			//if ((*neighbors_set_it) == 3)
			//{
			//	for (auto it : (captation_vertices_hashtable.at(*neighbors_set_it)->get_neighbors_set()))
			//	{
			//		std::cout << it->get_index() << std::endl;
			//	}
			//}
			std::set<int> neighbors_of_neighbor_idx_set = neighbor_indices_hashtable[*neighbors_set_it];

			// Computing the number of neighbors not already covered by the solution for the current neighbor 
			std::set<int> covered_neighbor_vertices_set;
			std::set_intersection(neighbors_of_neighbor_idx_set.begin(), neighbors_of_neighbor_idx_set.end(),
								  covered_vertices_set.begin(), covered_vertices_set.end(),
								  std::inserter(covered_neighbor_vertices_set, 
												covered_neighbor_vertices_set.begin()));
			current_degree = neighbors_of_neighbor_idx_set.size() - covered_neighbor_vertices_set.size();
			//std::cout << "nan si" << std::endl;
			//for (auto lol_it : covered_neighbor_vertices_set)
			//{
			//	std::cout << "    " << lol_it;
			//}
			//std::cout << std::endl;
			//std::cout << "        " << *neighbors_set_it << std::endl;
			//current_degree = captation_vertices_hashtable.at(*neighbors_set_it)->get_degree();
			if (current_degree > highest_degree)
			{
				highest_degree = current_degree;
				highest_degree_vertex_idx = *neighbors_set_it;
			}
		}
		//std::cout << std::endl;
		//std::cout << "highest degree vertex : " << highest_degree_vertex_idx << "   added vertices : " << highest_degree << std::endl;
		solution_vertices_set.insert(highest_degree_vertex_idx);
		std::set<int> neighbors_idx_set = neighbor_indices_hashtable[highest_degree_vertex_idx];
		//std::set<int> neighbors_idx_set = captation_graph.HeuristicRemoval(highest_degree_vertex_idx);
		covered_vertices_set.insert(highest_degree_vertex_idx);
		for (std::set<int>::iterator neighbors_idx_set_it = neighbors_idx_set.begin(); neighbors_idx_set_it != neighbors_idx_set.end(); neighbors_idx_set_it++)
		{
			//std::cout << "  oui      " << *neighbors_idx_set_it;
			covered_vertices_set.insert(*neighbors_idx_set_it);
		}
		//std::cout << std::endl;
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