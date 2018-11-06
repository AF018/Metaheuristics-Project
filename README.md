# Metaheuristics-Project
Heuristics and metaheuristics implementation for the corresponding course at the MPRO.
The code mainly consists of a heuristic and a simulated annealing algorithm.

## Grid and solution format

The grid file format is as follows, each line represents a vertex, with in order its index and its position in space :

```
 0  0.00 0.00 
 1  4.73 4.27
 2  3.61 7.26
 3  5.64 0.81
 4  8.55 14.55
 5  7.73 6.44
```

The solution file format is as follows, each line gives the value of one index included in the solution, no index is specified multiplle times. It does not have to be sorted : 

```
150
317
161
314
39
167
95
545
392
108
```

## Generating solutions
To generate solutions, create a file displayed this way :

```
PATH_TO_GRID_FILE
PATH_TO_SOLUTION_FILE
PATH_TO_FUTURE_HEURISTIC_SOLUTION
PATH_TO_FUTURE_SA_SOLUTION
CAPTATION_RADIUS COMMUNICATION_RADIUS
RECONSTRUCTION_THRESHOLD
SIMULATED_ANNEALING_ITERATION_NUMBER
INITIAL_TEMPERATURE DECREASING_COEFFICIENT FINAL_TEMPERATURE
```

In case you don't want to import an existing solution, you can create one with the heuristic implemented in the code. To do that, just replace the PATH_TO_SOLUTION_FILE line by 0.
If you want to import a solution, the third line is not important since it specifies the path to the heuristic solution, which is only computed when there is no imported solution.

To run the heuristic and the simulated annealing, you would use for instance (given the specified folders exist, as well as the grid file and the solution file) :

```
Instances/captANOR1500_15_100.dat
Heuristic_results/heuristic_captANOR1500_15_100.dat
0
Results/SA_captANOR1500_15_100.dat
2.0 2.0
1
1
6.2 0.9995 0.4
```

You can display the solution by calling the python script `draw_graph.py` with the values corresponding to the ones you gave in the parameters file :
```
python draw_graph.py --input PATH_TO_GRID --rcapt CAPTATION_RADIUS --rcom COMMUNICATION_RADIUS --solution PATH_TO_SOLUTION
```

Continue commenting the work

The containers in the simulated annealing are really badly chosen, change some of them if there's time

Add a descent at the end : selecct the vertices in random order and remove those which are not necessary (useful)

Maybe change the best solution if the current one has the same score as the best

Some methods aren't used, maybe delete them

Some vectors seem to be copied in the heuristic, that is not cool for complexity (but not really important also)