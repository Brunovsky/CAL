#ifndef UI_BASE_H___
#define UI_BASE_H___

#include "graph.h"

namespace ui {

bool discard();

void clear_screen();



int select_option(int max);

int select_iterations();



Vertex* select_vertex(bool may_be_accidented = true);

Vertex* select_source_vertex(bool may_be_accidented = true);

Vertex* select_target_vertex(Vertex* source, bool may_be_accidented = true, bool must_be_reachable = true);

Edge* select_edge(bool may_be_accidented = true);

}

#endif // UI_BASE_H___
