#include "Heuristics.h"

Solution NaiveHeuristic(const NeighborGraph& captation_graph, const NeighborGraph& communication_graph)
{
	vector<int> solution_vertices_vector;
	set<int> covered_vertices_set;
	vector<vector<int> > const & captation_edges_vector = captation_graph.get_edges_vector();
	// Vector keeping in memory the vertices it can cover if added to the solution
	vector<unordered_set<int> > covering_potential_vector(captation_edges_vector.size());
	for (int vertex_idx = 0; vertex_idx < covering_potential_vector.size(); vertex_idx++)
	{
		covering_potential_vector[vertex_idx] = unordered_set<int>(captation_edges_vector[vertex_idx].begin(),
																		captation_edges_vector[vertex_idx].end());
	}
	// First iteration
	int highest_potential_vertex_idx = 0;
	int highest_potential = -1;
	int current_potential = -1;
	for (int current_vertex_idx = 0; current_vertex_idx < communication_graph.get_vertices_number(); current_vertex_idx++)
	{
		current_potential = captation_edges_vector[current_vertex_idx].size();
		if (current_potential > highest_potential)
		{
			highest_potential = current_potential;
			highest_potential_vertex_idx = current_vertex_idx;
		}
	}
	solution_vertices_vector.push_back(highest_potential_vertex_idx);
	vector<int> const & neighbors_idx_vector = captation_edges_vector.at(highest_potential_vertex_idx);
	covered_vertices_set.insert(highest_potential_vertex_idx);
	for (vector<int>::const_iterator neighbors_idx_vector_it = neighbors_idx_vector.begin(); neighbors_idx_vector_it != neighbors_idx_vector.end(); neighbors_idx_vector_it++)
	{
		covered_vertices_set.insert(*neighbors_idx_vector_it);
		for (auto neighbors_of_neighbor_it : captation_edges_vector[*neighbors_idx_vector_it])
		{
			covering_potential_vector[neighbors_of_neighbor_it].erase(*neighbors_idx_vector_it);
		}
		covering_potential_vector[*neighbors_idx_vector_it].erase(highest_potential_vertex_idx);
	}
	// Iterations after the first one
	while (covered_vertices_set.size() < communication_graph.get_vertices_number())
	{
		set<int> const & neighbors_set = communication_graph.GetNeighbors(solution_vertices_vector);;
		//cout << "getting neighbors : " << solution_vertices_set.size() << endl;
		//cout << "Number of neighbors of the solution set : " << neighbors_set.size() << endl;
		//for (set<int>::iterator it = neighbors_set.begin(); it != neighbors_set.end(); it++)
		//{
		//	cout << *it << "  ";
		//}
		//cout << endl;
		// Going through the neighbors to see which vertex has the highest number of not covered neighbors
		set<int>::iterator neighbors_set_it = neighbors_set.begin();
		highest_potential_vertex_idx = 0;
		highest_potential = -1;
		current_potential = -1;
		// Computing the vertex index that can cover a maximal number of uncovered vertices;
		for (; neighbors_set_it != neighbors_set.end(); neighbors_set_it++)
		{
			current_potential = covering_potential_vector[*neighbors_set_it].size();
			if (current_potential > highest_potential)
			{
				highest_potential = current_potential;
				highest_potential_vertex_idx = *neighbors_set_it;
			}
			//cout << "Exploring vertex : " << *neighbors_set_it << endl;
		}
		//cout << endl;
		//cout << "highest degree vertex : " << highest_potential_vertex_idx << "   added vertices : " << highest_potential << endl;
		solution_vertices_vector.push_back(highest_potential_vertex_idx);
		vector<int> const & neighbors_idx_vector = captation_edges_vector.at(highest_potential_vertex_idx);
		covered_vertices_set.insert(highest_potential_vertex_idx);
		//cout << "Covered vertices and : " << neighbors_idx_vector.size() << endl;
		vector<int>::const_iterator neighbors_idx_vector_it = neighbors_idx_vector.begin();
		for (; neighbors_idx_vector_it != neighbors_idx_vector.end(); neighbors_idx_vector_it++)
		{
			//cout << "   " << *neighbors_idx_vector_it;
			covered_vertices_set.insert(*neighbors_idx_vector_it);
			for (auto neighbors_of_neighbor_it : captation_edges_vector[*neighbors_idx_vector_it])
			{
				covering_potential_vector[neighbors_of_neighbor_it].erase(*neighbors_idx_vector_it);
			}
			covering_potential_vector[*neighbors_idx_vector_it].erase(highest_potential_vertex_idx);
		}
		//cout << endl;
	}

	Solution heuristic_solution(communication_graph.get_vertices_number(), &captation_graph, &communication_graph);
	vector<int>::iterator solution_vertices_vector_it = solution_vertices_vector.begin();
	for (; solution_vertices_vector_it != solution_vertices_vector.end(); solution_vertices_vector_it++)
	{
		cout << " | " << *solution_vertices_vector_it;
		heuristic_solution.AddVertexToTheSolution(*solution_vertices_vector_it);
	}
	cout << endl;

	return heuristic_solution;
}