#include "graph.h"

Edge::Edge(int id, Vertex* source, Vertex* target, Road* road):
    _id(id), _source(source), _target(target), _road(road) {
    _source->_out.insert(this);
    _target->_in.insert(this);
}

int Edge::id() const {
    return _id;
}

Vertex* Edge::source() const {
    return _source;
}

Vertex* Edge::target() const {
    return _target;
}

Road* Edge::road() const {
    return _road;
}

bool Edge::is_accidented() const {
    return _accidented;
}

bool Edge::is_clear() const {
    return !_accidented;
}

bool Edge::fix() {
    if (_accidented) {
        _accidented = false;
        _graph->view_edge_reset(this);
        _source->_out.insert(this);
        _target->_in.insert(this);
        _source->_acc_out.erase(this);
        _target->_acc_in.erase(this);
        return true;
    } else {
        return false;
    }
}

bool Edge::accident() {
    if (!_accidented) {
        _accidented = true;
        _graph->view_edge_reset(this);
        _source->_out.erase(this);
        _target->_in.erase(this);
        _source->_acc_out.insert(this);
        _target->_acc_in.insert(this);
        return true;
    } else {
        return false;
    }
}

double Edge::length() const {
    return _graph->distance(_source, _target);
}

void Edge::set_weight(double weight) {
    _weight = weight;
}

double Edge::get_weight() const {
    return _weight;
}
