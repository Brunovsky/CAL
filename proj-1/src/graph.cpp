#include "graph.h"

#include <cassert>
#include <cmath>

bool Graph::within_bounds(int x, int y) const {
    return x >= 0 && y >= 0 && x <= _width && y <= _height;
}

Graph::Graph(int width, int height, double scale):
    _gv(new GraphViewer(width, height, false)),
    _width(width), _height(height), _scale(scale) {
    _gv->createWindow(VIEW_WIDTH_DEFAULT, VIEW_HEIGHT_DEFAULT);
    _gv->defineVertexColor(COLOR_CLEAR);
    _gv->defineEdgeColor(COLOR_CLEAR);
    _gv->defineVertexSize(VIEW_VERTEX_SIZE_CLEAR);
}

Graph::~Graph() {
    _gv->closeWindow();

    for (auto element : V) {
        delete element.second;
    }

    for (auto element : E) {
        delete element.second;
    }

    for (auto element : R) {
        delete element.second;
    }
}

void Graph::clear() const {
    for (auto element : V) {
        Vertex* vertex = element.second;
        vertex->set_priority(0);
        vertex->set_cost(0);
        vertex->set_path(nullptr);
    }
}

bool Graph::add_vertex(Vertex* vertex) {
    assert(vertex != nullptr && get_vertex(vertex->id()) == nullptr);

    if (!within_bounds(vertex->x(), vertex->y())) return false;

    V[vertex->id()] = vertex;
    _gv->addNode(vertex->id(), vertex->x(), vertex->y());
    vertex->_graph = this;

    view_vertex_reset(vertex);

    return true;
}

double Graph::distance(Vertex* v1, Vertex* v2) const {
    double x1 = v1->x(), x2 = v2->x();
    double y1 = v1->y(), y2 = v2->y();
    return _scale * std::hypot(x1 - x2, y1 - y2);
}

Vertex* Graph::get_vertex(int vid) const {
    auto it = V.find(vid);

    if (it == V.end()) {
        return nullptr;
    } else {
        return it->second;
    }
}

bool Graph::add_edge(Edge* edge) {
    assert(edge != nullptr && get_edge(edge->id()) == nullptr);

    E[edge->id()] = edge;
    _gv->addEdge(edge->id(), edge->source()->id(), edge->target()->id(), EdgeType::DIRECTED);
    edge->_graph = this;

    view_edge_reset(edge);

    return true;
}

Edge* Graph::get_edge(int eid) const {
    auto it = E.find(eid);

    if (it == E.end()) {
        return nullptr;
    } else {
        return it->second;
    }
}

Edge* Graph::get_edge(int vsource, int vtarget) const {
    Vertex* source = get_vertex(vsource);
    Vertex* target = get_vertex(vtarget);

    if (source == nullptr || target == nullptr) {
        return nullptr;
    } else {
        return source->edge_to(target);
    }
}

bool Graph::add_road(Road* road) {
    assert(road != nullptr && get_edge(road->id()) == nullptr);

    R[road->id()] = road;
    road->_graph = this;

    return true;
}

Road* Graph::get_road(int rid) const {
    auto it = R.find(rid);

    if (it == R.end()) {
        return nullptr;
    } else {
        return it->second;
    }
}

const std::unordered_map<int, Road*>& Graph::get_road_map() const {
    return R;
}

void Graph::regenerate() {
    for (auto element : V) {
        Vertex* vertex = element.second;

        for (Edge* edge : vertex->_in) {
            double weight = edge->get_weight();
            double rand = std::fmod(std::rand(), weight) - weight / 2;
            edge->set_weight(std::fmod(rand + weight, edge->length()));
        }
    }
}
