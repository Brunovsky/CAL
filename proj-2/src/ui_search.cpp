#include "ui_search.h"
#include "string_search.h"
#include "benchmark.h"
#include "ui_paths.h"
#include "paths.h"

#include <iostream>
#include <iomanip>
#include <cmath>

using namespace search;

namespace ui {

using roads_t = std::vector<Road*>;

static const std::string ui_string = "$ Road Search Algorithms\n"
    "** Exact road name search:\n"
    " 1 - Naive search\n"
    " 2 - Boyer-Moore search\n"
    " 3 - Boyer-Moore-Galil search\n"
    " 4 - Knuth-Morris-Pratt search\n"
    "** Approximate name matching:\n"
    " 5 - Levenshtein distance\n"
    " 6 - Restricted-Damerau distance\n"
    " 7 - Damerau distance\n"
    "** Fuzzy road name search:\n"
    " 8 - Levenshtein fuzzy search\n"
    " 9 - Restricted-Damerau fuzzy search\n"
    "10 - Damerau fuzzy search\n"
    "** **\n"
    "11 < return\n";

std::string select_road_name() {
    std::string name;
    std::cout << "Road name to search (empty to cancel): ";
    std::getline(std::cin, name);
    return name;
}

static Road* extract_road_from_found(const roads_t& roads) {
    // Trivial road selection cases
    if (roads.empty()) {
        std::cout << "No road was found." << std::endl;
        discard();
        return nullptr;
    }
    if (roads.size() == 1) {
        std::cout << "One road was found (chosen automatically):" << std::endl;
        std::cout << roads[0]->name() << std::endl;
        graph->view_road(roads[0], 1);
        discard();
        graph->reset();
        return roads[0];
    }

    std::size_t total = roads.size();
    std::size_t digits = std::floor(std::log10(total) + 1);

    std::cout << total << " roads were found: Choose one:" << std::endl;

    // Print the road options, and highlight them.
    for (std::size_t i = 0; i < total; ++i) {
        std::cout << std::setw(digits) << (i + 1) << " - " << roads[i]->name() << std::endl;
        graph->view_road(roads[i], i + 1);
    }

    int option = select_option(total);

    if (option == 0) {
        std::cout << "Cancelled." << std::endl;
        discard();
        graph->reset();
        return nullptr;
    } else {
        std::cout << "Chose: " << roads[option - 1]->name() << std::endl;
        discard();
        graph->reset();
        return roads[option - 1];
    }
}

template <typename T>
static Road* call_exact_search(exact_search_t<T> exact_search, const T& preprocessor) {
    // 1. Load road container.
    auto road_map = graph->get_road_map();

    // 2. We will collect here all the roads with minimum fuzzy distance
    roads_t roads_found;

    for (const auto& element : road_map) {
        Road* road = element.second;
        if (road->name().empty()) continue;

        // 3.1. Compute the matching indices.
        auto indices = exact_search(road->name(), preprocessor);
        
        // 3.2. If there is a matching index add the road to the container.
        if (!indices.empty()) {
            roads_found.push_back(road);
        }
    }

    // 4.1. Return the roads found
    return extract_road_from_found(roads_found);
}

static Road* call_distance(distance_function_t distance_function, const std::string& road_name) {
    // 1. Load road container.
    auto road_map = graph->get_road_map();

    // 2. We will collect here all the roads with minimum fuzzy distance
    roads_t roads_found;

    // The minimum distance found so far
    std::size_t minimum = -1;

    for (const auto& element : road_map) {
        Road* road = element.second;
        if (road->name().empty()) continue;

        // 3.1. Compute the distance of each road's name to the given one.
        std::size_t distance = distance_function(road_name, road->name());
        
        // 3.2. If the distance found is strictly the best, clear the container.
        if (distance < minimum) {
            roads_found.clear();
            minimum = distance;
        }

        // 3.3. If the distance found is the best, add the road to the container.
        if (distance == minimum) {
            roads_found.push_back(road);
        }
    }

    std::cout << "Minimal distance: " << minimum << std::endl;

    // 4.1. Return the roads found
    return extract_road_from_found(roads_found);
}

static Road* call_fuzzy_search(fuzzy_search_t fuzzy_search, const std::string& road_name) {
    // 1. Load road container.
    auto road_map = graph->get_road_map();

    // 2. We will collect here all the roads with minimum fuzzy distance
    roads_t roads_found;

    // The minimum distance found so far
    std::size_t minimum = -1;

    for (const auto& element : road_map) {
        Road* road = element.second;
        if (road->name().empty()) continue;

        // 3.1. Compute the distance of each road's name to the given one.
        std::size_t distance = fuzzy_search(road->name(), road_name);
        
        // 3.2. If the distance found is strictly the best, clear the container.
        if (distance < minimum) {
            roads_found.clear();
            minimum = distance;
        }

        // 3.3. If the distance found is the best, add the road to the container.
        if (distance == minimum) {
            roads_found.push_back(road);
        }
    }

    std::cout << "Minimal distance: " << minimum << std::endl;

    // 4.1. Return the roads found
    return extract_road_from_found(roads_found);
}

static Road* select_road_from(int option) {
    std::string road_name = select_road_name();
    if (road_name.empty()) return nullptr;

    Road* road = nullptr;

    switch (option) {
    case 1:
        road = call_exact_search(naive_search, road_name);
        break;
    case 2:
        road = call_exact_search(boyer_moore_search, boyer_moore<>(road_name));
        break;
    case 3:
        road = call_exact_search(boyer_moore_galil_search, boyer_moore<>(road_name));
        break;
    case 4:
        road = call_exact_search(knuth_morris_pratt_search, knuth_morris_pratt(road_name));
        break;
    case 5:
        road = call_distance(levenshtein_distance, road_name);
        break;
    case 6:
        road = call_distance(restricted_damerau_distance, road_name);
        break;
    case 7:
        road = call_distance(damerau_distance, road_name);
        break;
    case 8:
        road = call_fuzzy_search(levenshtein_fuzzy, road_name);
        break;
    case 9:
        road = call_fuzzy_search(restricted_damerau_fuzzy, road_name);
        break;
    case 10:
        road = call_fuzzy_search(damerau_fuzzy, road_name);
        break;
    }

    return road;
}

void road_search() {
    // 1.1. Clear screen
    clear_screen();
    graph->reset();
    std::cout << ui_string << std::endl;

    // 1.2. Select algorithm
    int option = select_option(11);
    if (option == 11 || option == 0) return;

    // 1.3. Select starting road
    std::cout << "Select starting road" << std::endl;
    Road* start_road = select_road_from(option);
    if (start_road == nullptr) {
        return road_search();
    }

    // 1.4. Starting vertex:
    Vertex* start_vertex = start_road->start_vertex();
    graph->view_vertex_select(start_vertex);


    // 1.5. Select ending road
    std::cout << "Select end road" << std::endl;
    Road* end_road = select_road_from(option);
    if (end_road == nullptr) {
        return road_search();
    }

    // 1.6. End vertex:
    Vertex* end_vertex = end_road->end_vertex();
    graph->view_vertex_select(start_vertex);
    graph->view_vertex_select(end_vertex);

    // 4. Check path
    paths::breadth_first_search(start_vertex);
    if (paths::get_path(start_vertex, end_vertex).empty()) {
        std::cout << "Unreachable..." << std::endl;
        graph->clear();
        discard();
        return road_search();
    } else {
        graph->clear();
        do_astar_search(start_vertex, end_vertex);

        // 5. Clear and restart
        std::cout << "Done..." << std::endl;
        discard();
        return road_search();
    }
}

}
