#ifndef PATHS_H___
#define PATHS_H___

#include "graph.h"

#include <vector>

namespace paths {

path_t get_path(Vertex* source, Vertex* target);

void breadth_first_search(Vertex* source);

void greedy_best_first_search(Vertex* source, Vertex* target);

void dijkstra_late_exit(Vertex* source, Vertex* target);

void dijkstra_early_exit(Vertex* source, Vertex* target);

void astar_search(Vertex* source, Vertex* target);

void dijkstra_weight(Vertex* source, Vertex* target);

}

#endif // PATHS_H___
