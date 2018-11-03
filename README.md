# Metaheuristics-Project
Heuristics and metaheuristics implementation for the corresponding course at the MPRO

## Generating solutions
To generate solutions, create a file displayed this way :

```
PATH_TO_GRID_FILE
PATH_TO_SOLUTION_FILE
PATH_TO_GRID_FILE
PATH_TO_GRID_FILE
CAPTATION_RADIUS COMMUNICATION_RADIUS
RECONSTRUCTION_THRESHOLD
SIMULATED_ANNEALING_ITERATION_NUMBER
INITIAL_TEMPERATURE DECREASING_COEFFICIENT FINAL_TEMPERATURE
```

In case you don't want to import an existing solution, you can create one with the heuristic implemented in the code. To do that, just replace the PATH_TO_SOLUTION_FILE line by 0.

Continue commenting the work

The containers in the simulated annealing are really badly chosen, change some of them if there's time

Add a descent at the end : selecct the vertices in random order and remove those which are not necessary (useful)

Maybe change the best solution if the current one has the same score as the best

Some methods aren't used, maybe delete them

Some vectors seem to be copied in the heuristic, that is not cool for complexity (but not really important also)