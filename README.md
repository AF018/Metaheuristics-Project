# Metaheuristics-Project
Heuristics and metaheuristics implementation for the corresponding course at the MPRO

The construction of the neighbor graph is not optimized for now, i.e. all distances between every pair of points are computed, which is a waste of time.

It may be more efficient to compute directly in the Solution class a set/map/vector of all the vertices constituting the solution because it is often asked, for instance in both checks.