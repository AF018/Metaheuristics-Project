#pragma once

#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>		/* time */

#include "Heuristics.h"

using namespace std;

const int UPPER_BOUND_VALUE = 1e9;

void TabuSearch(Solution solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);
