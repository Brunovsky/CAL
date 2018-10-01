#include "ui_base.h"
#include "paths.h"
#include "benchmark.h"

#include <limits>
#include <regex>
#include <string>
#include <iostream>
#include <stdexcept>

namespace ui {

static const std::regex regex_quit(R"Z(\s*(?:esc|quit|q)[\.,;]?\s*)Z", std::regex::icase);

static int get_numeric_input(const std::string& input, int max = -1) {
    static const std::regex regex_int(R"Z(\s*(\d{1,8})[.,;]?\s*)Z");
    std::smatch match;

    if (std::regex_match(input, match, regex_int)) {
        int value = std::stoi(match[1]);

        return value != 0 && !(max > 0 && value > max);
    } else {
        return false;
    }
}

#ifdef _WIN32
    #include <cstdlib>

    void clear_screen() {
        std::system("cls");
    }
#else
    void clear_screen() {
        std::cout << "\033[2J\033[1;1H";
    }
#endif

bool discard() {
    std::string read;
    std::getline(std::cin, read);
    return read.empty();
}

int select_option(int max) {
    while (true) {
        std::string input;
        std::cout << "Select option: ";
        std::getline(std::cin, input);

        if (get_numeric_input(input, max)) {
            return std::stoi(input);
        } else if (std::regex_match(input, regex_quit)) {
            return 0;
        }
    }
}

int select_iterations() {
    while (true) {
        std::string input;
        std::cout << "Number of iterations: ";
        std::getline(std::cin, input);

        if (get_numeric_input(input)) {
            return std::stoi(input);
        } else if (std::regex_match(input, regex_quit)) {
            return 0;
        }
    }
}

Vertex* select_vertex(bool may_be_accidented) {
    graph->show_all_vertex_ids();
    Vertex* selected;

    while (true) {
        std::string input;
        std::cout << "Select vertex (q to quit): ";
        std::getline(std::cin, input);

        if (get_numeric_input(input)) {
            int id = std::stoi(input);
            Vertex* vertex = graph->get_vertex(id);

            if (vertex != nullptr) { // Vertex found
                if (!may_be_accidented && vertex->is_accidented()) {
                    std::cout << " Error: Accidented vertex." << std::endl;
                } else { // All goochy
                    selected = vertex;
                    break;
                }
            } else { // Vertex not found
                std::cout << " Error: Vertex not found." << std::endl;
            }
        } else if (std::regex_match(input, regex_quit)) { // Esc-type input, exit
            selected = nullptr;
            break;
        }
    }

    graph->hide_all_vertex_ids();
    return selected;
}

Vertex* select_source_vertex(bool may_be_accidented) {
    graph->show_all_vertex_ids();
    Vertex* selected;

    while (true) {
        std::string input;
        std::cout << "Select source vertex (q to quit): ";
        std::getline(std::cin, input);

        if (get_numeric_input(input)) {
            int id = std::stoi(input);
            Vertex* vertex = graph->get_vertex(id);

            if (vertex != nullptr) { // Vertex found
                if (!may_be_accidented && vertex->is_accidented()) {
                    std::cout << " Error: Accidented vertex." << std::endl;
                } else if (vertex->out_degree() == 0) {
                    std::cout << " Error: No reachable nodes." << std::endl;
                } else { // All goochy
                    selected = vertex;
                    break;
                }
            } else { // Vertex not found
                std::cout << " Error: Vertex not found." << std::endl;
            }
        } else if (std::regex_match(input, regex_quit)) { // Esc-type input, exit
            selected = nullptr;
            break;
        }
    }

    graph->hide_all_vertex_ids();
    return selected;
}

Vertex* select_target_vertex(Vertex* source, bool may_be_accidented, bool must_be_reachable) {
    graph->show_all_vertex_ids();
    Vertex* selected;

    if (must_be_reachable) paths::breadth_first_search(source);

    while (true) {
        std::string input;
        std::cout << "Select target vertex (q to quit): ";
        std::getline(std::cin, input);

        if (get_numeric_input(input)) {
            int id = std::stoi(input);
            Vertex* vertex = graph->get_vertex(id);

            if (vertex != nullptr) { // Vertex found
                if (!may_be_accidented && vertex->is_accidented()) {
                    std::cout << " Error: Accidented vertex." << std::endl;
                } else if (vertex->get_path() == nullptr) {
                    std::cout << " Error: Not reachable." << std::endl;
                } else { // All goochy
                    selected = vertex;
                    break;
                }
            } else { // Vertex not found
                std::cout << " Error: Vertex not found." << std::endl;
            }
        } else if (std::regex_match(input, regex_quit)) { // Esc-type input, exit
            selected = nullptr;
            break;
        }
    }

    if (must_be_reachable) graph->clear();

    graph->hide_all_vertex_ids();
    return selected;
}

Edge* select_edge(bool may_be_accidented) {
    graph->show_all_edge_ids();
    Edge* selected;

    while (true) {
        std::string input;
        std::cout << "Select edge (q to quit): ";
        std::getline(std::cin, input);

        if (get_numeric_input(input)) {
            int id = std::stoi(input);
            Edge* edge = graph->get_edge(id);

            if (edge != nullptr) { // Edge found
                if (!may_be_accidented && edge->is_accidented()) {
                    std::cout << " Error: Accidented edge." << std::endl;
                } else { // All goochy
                    selected = edge;
                    break;
                }
            } else { // Edge not found
                std::cout << " Error: Edge not found." << std::endl;
            }
        } else if (std::regex_match(input, regex_quit)) { // Esc-type input, exit
            selected = nullptr;
            break;
        }
    }

    graph->hide_all_edge_ids();
    return selected;
}

}
