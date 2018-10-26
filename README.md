# Metaheuristics-Project
Heuristics and metaheuristics implementation for the corresponding course at the MPRO

Some vectors seem to be copied in the heuristic, that is not cool for complexity
The edge vectors might be already ordered, must be verified
Checj if all the vectors used in the heuristic are ordered (if necessary) and have no repeated values in em
Also for BFS, use vector instead of map, it will gain some time
Option to display vertices when constructing ?

Display the vertices of the graph with its neghbors and its index for sanity check

The construction of the neighbor graph is not optimized for now, i.e. all distances between every pair of points are computed, which is a waste of time.

It may be more efficient to compute directly in the Solution class a set/map/vector of all the vertices constituting the solution because it is often asked, for instance in both checks.

Also interesting to randomize the behavior of the heuristic.
The heuristic goes through the degree vector to find the biggest, which is not deterministic and not great also.

There are changes that need to be made in the construction of the TargetNet in this case