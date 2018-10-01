#include "ui_paths.h"
#include "paths.h"
#include "benchmark.h"

#include <limits>
#include <iostream>
#include <string>
#include <vector>

#define UPDATE_SPEED_1 200.0
#define UPDATE_SPEED_2 100.0

namespace ui {

using namespace paths;

static const std::string ui_string = "$ Shortest Path Algorithms\n"
    " 1 - Greedy Best-First Search\n"
    " 2 - Dijkstra Late Exit\n"
    " 3 - Dijkstra Early Exit\n"
    " 4 - A*\n"
    " 5 - Simulation (edge by edge)\n"
    " 6 - Simulation (road by road)\n"
    " 7 - Benchmark GBFS, Dijkstra and A*\n"
    " 8 < return\n";

static void animate_one_edge(path_t path, Vertex* &current) {
    graph->view_vertex_custom(current, PATH_COLOR_1);

    // The new current is the next vertex in the path
    current = path.at(1);

    path_t first = get_path(path.front(), path.at(1));
    path_t second = get_path(path.at(1), path.back());

    graph->animate_path(first, 200, PATH_COLOR_1);
    graph->animate_path(second, 15, PATH_COLOR_2);
}

static void animate_one_road(path_t path, Vertex* &current) {
    graph->view_vertex_custom(current, PATH_COLOR_1);

    // This is the road we should stay on
    Edge* first_edge = path.at(0)->edge_to(path.at(1));
    Road* first_road = first_edge->road();

    // Find the vertex in which we switch Roads
    // If the entire path is contained in one Road, i.e. this is the
    // last road and the for cycle ends naturally, then
    // the new current is the last (destination) vertex.
    current = path.back();
    for (size_t i = 1; i < path.size(); ++i) {
        Edge* edge = path.at(i - 1)->edge_to(path.at(i));
        Road* road = edge->road();
        if (road != first_road) {
            current = path.at(i - 1);
            break;
        }
    }

    path_t first = get_path(path.front(), current);
    path_t second = get_path(current, path.back());

    graph->animate_path(first, 200, PATH_COLOR_1);
    graph->animate_path(second, 15, PATH_COLOR_2);
}

void do_greedy_best_first_search(Vertex* source, Vertex* target) {
    // Perform GBFS
    greedy_best_first_search(source, target);
    path_t path = get_path(source, target);
    graph->clear();

    // Animate GBFS
    graph->view_vertex_select(source);
    graph->animate_path(path, UPDATE_SPEED_1, PATH_COLOR_2);
    graph->view_vertex_select(target);
    graph->update();

    // Perform A* (to verify)
    astar_search(source, target);
    path_t astarpath = get_path(source, target);
    graph->clear();

    // Evaluate
    if (equal(path.cbegin(), path.cend(), astarpath.cbegin(), astarpath.cend())) {
        std::cout << "The path found is the best path." << std::endl;
    } else {
        std::cout << "The path found is NOT the best path. We show the best path next." << std::endl;
        discard();
        graph->animate_path(astarpath, UPDATE_SPEED_2, PATH_COLOR_1);
        graph->view_vertex_select(target);
    }
    //discard();
}

void do_dijkstra_late_exit_search(Vertex* source, Vertex* target) {
    // Perform Dijkstra
    dijkstra_late_exit(source, target);
    path_t path = get_path(source, target);
    graph->clear();

    // Animate Dijkstra
    graph->view_vertex_select(source);
    graph->animate_path(path, UPDATE_SPEED_1, PATH_COLOR_1);
    graph->view_vertex_select(target);
    graph->update();
    //discard();
}

void do_dijkstra_early_exit_search(Vertex* source, Vertex* target) {
    // Perform Dijkstra
    dijkstra_early_exit(source, target);
    path_t path = get_path(source, target);
    graph->clear();

    // Animate Dijkstra
    graph->view_vertex_select(source);
    graph->animate_path(path, UPDATE_SPEED_1, PATH_COLOR_1);
    graph->view_vertex_select(target);
    graph->update();
    //discard();
}

void do_astar_search(Vertex* source, Vertex* target) {
    // Perform A*
    astar_search(source, target);
    path_t path = get_path(source, target);
    graph->clear();

    // Animate Dijkstra
    graph->view_vertex_select(source);
    graph->animate_path(path, UPDATE_SPEED_1, PATH_COLOR_1);
    graph->view_vertex_select(target);
    graph->update();
    //discard();
}

void do_edge_simulation(Vertex* source, Vertex* target) {
    while (true) {
        dijkstra_weight(source, target);
        path_t path = get_path(source, target);
        animate_one_edge(path, source);
        if (!discard()) break;

        if (source == target) break;
        path = get_path(source, target);
        graph->clear_path(path, 0);

        // Simulate
        graph->regenerate();
    }
}

void do_road_simulation(Vertex* source, Vertex* target) {
    while (true) {
        dijkstra_weight(source, target);
        path_t path = get_path(source, target);
        animate_one_road(path, source);
        if (!discard()) break;

        if (source == target) break;
        path = get_path(source, target);
        graph->clear_path(path, 0);

        // Simulate
        graph->regenerate();
    }
}

void do_benchmark(Vertex* source, Vertex* target) {
    int iterations = select_iterations();
    if (iterations == 0) return;

    // Perform A* to find the best path and show it, and
    // only the proceed.
    astar_search(source, target);
    path_t path = get_path(source, target);
    graph->view_vertex_select(source);
    graph->animate_path(path, 0, PATH_COLOR_1);
    graph->view_vertex_select(target);

    std::cout << "=== Benchmark " << iterations << " iterations ===\n" << std::endl;

    // Silent warmup, bring the relevant memory into the cache
    for (int i = 0; i < std::min(10, iterations / 10); ++i) {
        greedy_best_first_search(source, target);
        graph->clear();
        dijkstra_late_exit(source, target);
        graph->clear();
        dijkstra_early_exit(source, target);
        graph->clear();
        astar_search(source, target);
        graph->clear();
    }

    // Greedy Best First Search
    {
        micro_t time = 0us;

        for (int i = 0; i < iterations; ++i) {
            now_t start = time_now();
            greedy_best_first_search(source, target);
            now_t end = time_now();
            time += time_diff(start, end);
            graph->clear();
        }

        auto total = time.count() / iterations;

        std::cout << "--- (1) Greedy Best First Search ---" << std::endl;
        std::cout << "Average Time: " << total << " microseconds." << std::endl;
    }

    // Check if Greedy Best First Search found best path
    {
        greedy_best_first_search(source, target);
        path_t gbfspath = get_path(source, target);
        graph->clear();

        if (equal(gbfspath.cbegin(), gbfspath.cend(), path.cbegin(), path.cend())) {
            std::cout << "The path found by GBFS is the best path." << std::endl;
        } else {
            std::cout << "The path found by GBFS is NOT the best path." << std::endl;
            graph->animate_path(gbfspath, 0, PATH_COLOR_2);
        }
    }

    // Late Exit Dijkstra
    {
        micro_t time = 0us;

        for (int i = 0; i < iterations; ++i) {
            now_t start = time_now();
            dijkstra_late_exit(source, target);
            now_t end = time_now();
            time += time_diff(start, end);
            graph->clear();
        }

        auto total = time.count() / iterations;

        std::cout << "--- (2) Late Exit Dijkstra ---" << std::endl;
        std::cout << "Average Time: " << total << " microseconds." << std::endl;
    }

    // Early Exit Dijkstra
    {
        micro_t time = 0us;

        for (int i = 0; i < iterations; ++i) {
            now_t start = time_now();
            dijkstra_early_exit(source, target);
            now_t end = time_now();
            time += time_diff(start, end);
            graph->clear();
        }

        auto total = time.count() / iterations;

        std::cout << "--- (3) Early Exit Dijkstra ---" << std::endl;
        std::cout << "Average Time: " << total << " microseconds." << std::endl;
    }

    // Benchmark A*
    {
        micro_t time = 0us;

        for (int i = 0; i < iterations; ++i) {
            now_t start = time_now();
            astar_search(source, target);
            now_t end = time_now();
            time += time_diff(start, end);
            graph->clear();
        }

        auto total = time.count() / iterations;

        std::cout << "--- (4) A* Search ---" << std::endl;
        std::cout << "Average Time: " << total << " microseconds." << std::endl;
    }

    //discard();
}

void paths() {
    clear_screen();
    graph->reset();
    std::cout << ui_string << std::endl;

    int option = select_option(8);
    if (option == 8 || option == 0) return;

    Vertex* source = select_source_vertex(true);
    if (source == nullptr) {
        return;
    }

    graph->color_unreachable(source);
    graph->view_vertex_select(source);
    graph->update();

    Vertex* target = select_target_vertex(source);
    if (target == nullptr) {
        graph->reset();
        return;
    }

    graph->view_vertex_select(target);

    switch (option) {
    case 1:
        do_greedy_best_first_search(source, target);
        break;
    case 2:
        do_dijkstra_late_exit_search(source, target);
        break;
    case 3:
        do_dijkstra_early_exit_search(source, target);
        break;
    case 4:
        do_astar_search(source, target);
        break;
    case 5:
        do_edge_simulation(source, target);
        break;
    case 6:
        do_road_simulation(source, target);
        break;
    case 7:
        do_benchmark(source, target);
        break;
    }

    std::cout << "Done..." << std::endl;
    discard();
    return paths();
}

}
