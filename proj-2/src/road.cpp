#include "graph.h"

#include <algorithm>
#include <cassert>

Road::Road(int id, std::string name, bool bothways):
    _id(id), _name(name), _bothways(bothways) {}

int Road::id() const {
    return _id;
}

const std::string& Road::name() const {
    return _name;
}

bool Road::bothways() const {
    return _bothways;
}

double Road::total_length() const {
    double accumulator = 0;
    for (Edge* edge : _forward) {
        accumulator += edge->length();
    }
    return accumulator;
}

bool Road::is_accidented(bool forward) const {
    static const auto acc_funct = [](Edge* e) { return e->is_accidented(); };
    if (forward) {
        return std::any_of(_forward.begin(), _forward.end(), acc_funct);
    } else {
        return std::any_of(_backward.begin(), _backward.end(), acc_funct);
    }
}

bool Road::is_clear(bool forward) const {
    static const auto clear_funct = [](Edge* e) { return e->is_clear(); };
    if (forward) {
        return std::all_of(_forward.begin(), _forward.end(), clear_funct);
    } else {
        return std::all_of(_backward.begin(), _backward.end(), clear_funct);
    }
}

bool Road::add_edge(Edge* edge, bool forward) {
    if (forward) {
        _forward.push_back(edge);
    } else {
        _backward.push_back(edge);
    }
    return true;
}

Vertex* Road::start_vertex() const {
    return _forward.front()->source();
}

Vertex* Road::end_vertex() const {
    return _forward.back()->target();
}
