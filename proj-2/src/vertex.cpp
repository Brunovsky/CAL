#include "graph.h"

Vertex::Vertex(int id, int x, int y):
    _id(id), _x(x), _y(y) {}

int Vertex::id() const {
    return _id;
}

int Vertex::x() const {
    return _x;
}

int Vertex::y() const {
    return _y;
}

bool Vertex::is_accidented() const {
    return _accidented;
}

bool Vertex::is_clear() const {
    return !_accidented;
}

bool Vertex::fix() {
    if (_accidented) {
        _accidented = false;
        _graph->view_vertex_reset(this);
        return true;
    } else {
        return false;
    }
}

bool Vertex::accident() {
    if (!_accidented) {
        _accidented = true;
        _graph->view_vertex_reset(this);
        return true;
    } else {
        return false;
    }
}

Edge* Vertex::edge_to(Vertex* other) const {
    for (Edge* edge : _out) {
        if (edge->target() == other) return edge;
    }

    for (Edge* edge : _acc_out) {
        if (edge->target() == other) return edge;
    }

    return nullptr;
}

Edge* Vertex::edge_from(Vertex* other) const {
    for (Edge* edge : _in) {
        if (edge->target() == other) return edge;
    }

    for (Edge* edge : _acc_in) {
        if (edge->target() == other) return edge;
    }

    return nullptr;
}

bool Vertex::connects_to(Vertex* other) const {
    return edge_to(other) != nullptr;
}

bool Vertex::connects_from(Vertex* other) const {
    return edge_from(other) != nullptr;
}

std::size_t Vertex::in_degree() const {
    return _in.size();
}

std::size_t Vertex::out_degree() const {
    return _out.size();
}

const std::unordered_set<Edge*>& Vertex::incident() const {
    return _in;
}

const std::unordered_set<Edge*>& Vertex::outgoing() const {
    return _out;
}

double Vertex::distance(Vertex* other) const {
    return _graph->distance(const_cast<Vertex*>(this), other);
}

void Vertex::set_path(Vertex* previous) {
    _path = previous;
}

void Vertex::set_priority(double priority) {
    _priority = priority;
}

void Vertex::set_cost(double cost) {
    _cost = cost;
}

Vertex* Vertex::get_path() const {
    return _path;
}

double Vertex::get_priority() const {
    return _priority;
}

double Vertex::get_cost() const {
    return _cost;
}

bool operator<(const Vertex& v1, const Vertex& v2) {
    return v1.get_priority() < v2.get_priority();
}
