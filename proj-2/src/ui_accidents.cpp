#include "ui_accidents.h"

#include <iostream>
#include <string>

namespace ui {

static const std::string accident_string = "$ Cause accidents\n"
    "1 - Vertex accidents\n"
    "2 - Edge accidents\n"
    "3 < return\n";

static const std::string fix_string = "$ Fix accidents\n"
    "1 - Vertex accidents\n"
    "2 - Edge accidents\n"
    "3 < return\n";

static void vertex_accident() {
    Vertex* vertex = select_vertex(true);
    if (vertex == nullptr) return;

    // Cause accident
    vertex->accident();
    graph->update();
}

static void edge_accident() {
    Edge* edge = select_edge(true);
    if (edge == nullptr) return;

    // Cause accident
    edge->accident();
    graph->update();
}

static void vertex_fix() {
    Vertex* vertex = select_vertex(true);
    if (vertex == nullptr) return;

    // Fix node
    vertex->fix();
    graph->update();
}

static void edge_fix() {
    Edge* edge = select_edge(true);
    if (edge == nullptr) return;

    // Fix edge
    edge->fix();
    graph->update();
}

void make_accident() {
    std::cout << accident_string << std::endl;

    int option = select_option(3);
    if (option == 3) return;

    switch (option) {
    case 1:
        vertex_accident();
        break;
    case 2:
        edge_accident();
        break;
    }

    return make_accident();
}

void make_fix() {
    std::cout << fix_string << std::endl;

    int option = select_option(3);
    if (option == 3) return;

    switch(option) {
    case 1:
        vertex_fix();
        break;
    case 2:
        edge_fix();
        break;
    }

    return make_fix();
}

}
