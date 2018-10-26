#include "Heuristics.h"

Solution NaiveHeuristic(NeighborGraph captation_graph, const NeighborGraph& communication_graph)
{
	std::vector<int> solution_vertices_vector;
	std::set<int> covered_vertices_set;
	std::vector<std::vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	// Vector keeping in memory the vertices it can cover if added to the solution
	//std::vector<std::vector<int> > covering_number_vector = captation_graph.get_edges_vector();
	// First iteration
	int highest_degree_vertex_idx = 0;
	int highest_degree = -1;
	int current_degree = -1;
	for (int current_vertex_idx = 0; current_vertex_idx < communication_graph.get_vertices_number(); current_vertex_idx++)
	{
		//std::vector<int> neighbors_vector(captation_graph.get_vertices_number());
		//std::iota(neighbors_vector.begin(), neighbors_vector.end(), 0);
		current_degree = captation_edges_vector[current_vertex_idx].size();
		if (current_degree > highest_degree)
		{
			highest_degree = current_degree;
			highest_degree_vertex_idx = current_vertex_idx;
		}
	}
	solution_vertices_vector.push_back(highest_degree_vertex_idx);
	std::vector<int> neighbors_idx_vector = captation_edges_vector.at(highest_degree_vertex_idx);
	covered_vertices_set.insert(highest_degree_vertex_idx);
	for (std::vector<int>::iterator neighbors_idx_vector_it = neighbors_idx_vector.begin(); neighbors_idx_vector_it != neighbors_idx_vector.end(); neighbors_idx_vector_it++)
	{
		covered_vertices_set.insert(*neighbors_idx_vector_it);
	}
	// Iterations after the first one
	while (covered_vertices_set.size() < communication_graph.get_vertices_number())
	{
		std::set<int> neighbors_set;
		//std::cout << "getting neighbors : " << solution_vertices_set.size() << std::endl;
		neighbors_set = communication_graph.GetNeighbors(solution_vertices_vector);
		//std::cout << "Number of neighbors of the solution set : " << neighbors_set.size() << std::endl;
		//for (std::set<int>::iterator it = neighbors_set.begin(); it != neighbors_set.end(); it++)
		//{
		//	std::cout << *it << "  ";
		//}
		//std::cout << std::endl;
		// Going through the neighbors to see which vertex has the highest number of not covered neighbors
		std::set<int>::iterator neighbors_set_it = neighbors_set.begin();
		int highest_degree_vertex_idx = 0;
		int highest_degree = -1;
		int current_degree = -1;
		//std::cout << "computing the highest degree" << std::endl;
		for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			//if ((*neighbors_set_it) == 4)
			//{
			//	for (auto it : (captation_edges_vector.at(*neighbors_set_it)))
			//	{
			//		std::cout << it << std::endl;
			//	}
			//}
			//std::cout << "neighbor of neighbor vector" << std::endl;
			std::vector<int> neighbors_of_neighbor_idx_vector = captation_edges_vector.at(*neighbors_set_it);

			// Computing the number of neighbors not already covered by the solution for the current neighbor 
			std::vector<int> covered_neighbor_vertices_vector;
			//std::cout << "computing intersection" << std::endl;
			//for (auto it : neighbors_of_neighbor_idx_vector)
			//{
			//	std::cout << it << std::endl;;
			//}
			//std::cout << std::endl;;
			//for (auto it : covered_vertices_set)
			//{
			//	std::cout << it << std::endl;
			//}
			std::set_intersection(neighbors_of_neighbor_idx_vector.begin(), neighbors_of_neighbor_idx_vector.end(),
								  covered_vertices_set.begin(), covered_vertices_set.end(),
								  back_inserter(covered_neighbor_vertices_vector));
			current_degree = neighbors_of_neighbor_idx_vector.size() - covered_neighbor_vertices_vector.size();
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
		solution_vertices_vector.push_back(highest_degree_vertex_idx);
		std::vector<int> neighbors_idx_vector = captation_edges_vector.at(highest_degree_vertex_idx);
		//std::set<int> neighbors_idx_set = captation_graph.HeuristicRemoval(highest_degree_vertex_idx);
		covered_vertices_set.insert(highest_degree_vertex_idx);
		//std::cout << "Covered vertices" << std::endl;
		for (std::vector<int>::iterator neighbors_idx_vector_it = neighbors_idx_vector.begin(); neighbors_idx_vector_it != neighbors_idx_vector.end(); neighbors_idx_vector_it++)
		{
			//std::cout << "   " << *neighbors_idx_vector_it;
			covered_vertices_set.insert(*neighbors_idx_vector_it);
		}
		//std::cout << std::endl;
	}

	Solution heuristic_solution(communication_graph.get_vertices_number(), false);
	std::vector<int>::iterator solution_vertices_vector_it = solution_vertices_vector.begin();
	for (; solution_vertices_vector_it != solution_vertices_vector.end(); solution_vertices_vector_it++)
	{
		std::cout << " | " << *solution_vertices_vector_it;
		heuristic_solution.AddVertexToTheSolution(*solution_vertices_vector_it);
	}
	std::cout << std::endl;

	return heuristic_solution;
}