# Metaheuristics-Project
Heuristics and metaheuristics implementation for the corresponding course at the MPRO

Display the vertices of the graph with its neghbors and its index for sanity check

The construction of the neighbor graph is not optimized for now, i.e. all distances between every pair of points are computed, which is a waste of time.
Changing sets to unordered_sets (if it does not affect the algorithms) is a good way to improve the complexity for basic tasks

It may be more efficient to compute directly in the Solution class a set/map/vector of all the vertices constituting the solution because it is often asked, for instance in both checks.

Also interesting to randomize the behavior of the heuristic.
The heuristic goes through the degree vector to find the biggest, which is not deterministic and not great also.

With the unordered_map, the initial label value can be the same than the one used in the code
There are changes that need to be made in the construction of the TargetNet in this case