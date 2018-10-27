#pragma once

#include <stdio.h>      /* NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>		/* time */

#include "Heuristics.h"

using namespace std;

void TabooSearch(Solution solution, const NeighborGraph& captation_graph, const NeighborGraph& communication_graph);
