#ifndef UI_PATHS_H___
#define UI_PATHS_H___

#include "ui_base.h"

namespace ui {

void do_greedy_best_first_search(Vertex* source, Vertex* target);

void do_dijkstra_late_exit_search(Vertex* source, Vertex* target);

void do_dijkstra_early_exit_search(Vertex* source, Vertex* target);

void do_astar_search(Vertex* source, Vertex* target);

void do_edge_simulation(Vertex* source, Vertex* target);

void do_road_simulation(Vertex* source, Vertex* target);

void do_benchmark(Vertex* source, Vertex* target);

void paths();

}

#endif // UI_PATHS_H___
