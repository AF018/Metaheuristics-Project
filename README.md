# Metaheuristics-Project
Heuristics and metaheuristics implementation for the corresponding course at the MPRO

Some vectors seem to be copied in the heuristic, that is not cool for complexity
The edge vectors might be already ordered, must be verified
Consider the well as a a starting point in the BFS, might cause trouble otherwise
Also for BFS, use vector instead of map, it will gain some time
GetNeighbors recupere aussi les elements voisins qui sont dans l'ensemble de base, changer et renommer en GetCoveredVertices
Option to display vertices when constructing ?

Display the vertices of the graph with its neghbors and its index for sanity check

The construction of the neighbor graph is not optimized for now, i.e. all distances between every pair of points are computed, which is a waste of time.

It may be more efficient to compute directly in the Solution class a set/map/vector of all the vertices constituting the solution because it is often asked, for instance in both checks.

Also interesting to randomize the behavior of the heuristic.
The heuristic goes through the degree vector to find the biggest, which is not deterministic and not great also.

There are changes that need to be made in the construction of the TargetNet in this case