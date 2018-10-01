#include "graph.h"
#include "paths.h"

#include <cmath>
#include <unordered_set>

// https://stackoverflow.com/questions/1658386/sleep-function-in-c
#ifdef _WIN32
    #include <windows.h>

    static void sleep_for(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }
#else
    #include <unistd.h>

    static void sleep_for(unsigned milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif

/**
 * In this sets we store the vertices and edges modified by any of the functions,
 * to be reset by Graph::reset()
 */
static std::unordered_set<Vertex*> modified_vertex;
static std::unordered_set<Edge*> modified_edge;

void Graph::view_vertex_custom(Vertex* vertex, Color color) const {
    _gv->setVertexColor(vertex->id(), color);

    _gv->setVertexSize(vertex->id(), VIEW_VERTEX_SIZE_CUSTOM);

    modified_vertex.insert(vertex);
}

void Graph::view_vertex_select(Vertex* vertex) const {
    _gv->setVertexColor(vertex->id(), COLOR_SELECTED);

    _gv->setVertexSize(vertex->id(), VIEW_VERTEX_SIZE_SELECTED);

    //_gv->setVertexLabel(vertex->id(), std::to_string(vertex->id()));

    modified_vertex.insert(vertex);
}

void Graph::view_vertex_reset(Vertex* vertex) const {
    _gv->setVertexColor(vertex->id(),
        vertex->is_clear() ? COLOR_CLEAR : COLOR_ACCIDENTED);

    _gv->setVertexSize(vertex->id(),
        vertex->is_clear() ? VIEW_VERTEX_SIZE_CLEAR : VIEW_VERTEX_SIZE_ACCIDENTED);

    // Might not be necessary.
    _gv->clearVertexLabel(vertex->id());

    modified_vertex.erase(vertex);
}

void Graph::view_vertex_label(Vertex* vertex, int label) const {
    _gv->setVertexLabel(vertex->id(), std::to_string(label));

    modified_vertex.insert(vertex);
}

void Graph::view_edge_custom(Edge* edge, Color color) const {
    _gv->setEdgeColor(edge->id(), color);

    _gv->setEdgeThickness(edge->id(), VIEW_EDGE_THICKNESS_CUSTOM);

    modified_edge.insert(edge);
}

void Graph::view_edge_select(Edge* edge) const {
    _gv->setEdgeColor(edge->id(), COLOR_SELECTED);

    _gv->setEdgeThickness(edge->id(), VIEW_EDGE_THICKNESS_SELECTED);

    //_gv->setEdgeLabel(vertex->id(), std::to_string(vertex->id()));

    modified_edge.insert(edge);
}

void Graph::view_edge_reset(Edge* edge) const {
    _gv->setEdgeColor(edge->id(),
        edge->is_clear() ? COLOR_CLEAR : COLOR_ACCIDENTED);

    _gv->setEdgeThickness(edge->id(), 
        edge->is_clear() ? VIEW_EDGE_THICKNESS_CLEAR : VIEW_EDGE_THICKNESS_ACCIDENTED);

    // Might not be necessary.
    _gv->clearEdgeLabel(edge->id());

    modified_edge.erase(edge);
}

void Graph::view_edge_label(Edge* edge, int label) const {
    _gv->setEdgeLabel(edge->id(), std::to_string(label));

    modified_edge.insert(edge);
}

void Graph::view_road(Road* road, int label) const {
    // We cycle through this array to pick the colors.
    static constexpr std::size_t road_colors_n = 7;
    static std::size_t current_road_color = 0;
    static Color road_colors[] = {BLUE, RED, GREEN, ORANGE, MAGENTA, YELLOW, CYAN};
    
    for (std::size_t i = 0; i < road->_forward.size(); ++i) {
        Edge* edge = road->_forward[i];
        Vertex* vertex = edge->source();
        view_vertex_custom(vertex, road_colors[current_road_color]);
        view_edge_custom(edge, road_colors[current_road_color]);
        view_edge_label(edge, label);
    }
    view_vertex_custom(road->end_vertex(), road_colors[current_road_color]);
    current_road_color = (current_road_color + 1) % road_colors_n;
    update();
}

void Graph::update() const {
    _gv->rearrange();
}

void Graph::reset() const {
    for (Vertex* vertex : modified_vertex) {
        view_vertex_reset(vertex);
    }

    for (Edge* edge : modified_edge) {
        view_edge_reset(edge);
    }

    update();
}

void Graph::show_all_vertex_ids() const {
    for (auto element : V) {
        int id = element.first;
        _gv->setVertexLabel(id, std::to_string(id));
    }
    update();
}

void Graph::hide_all_vertex_ids() const {
    for (auto element : V) {
        int id = element.first;
        _gv->clearVertexLabel(id);
    }
    update();
}

void Graph::show_all_edge_ids() const {
    for (auto element : E) {
        int id = element.first;
        _gv->setEdgeLabel(id, std::to_string(id));
    }
    update();
}

void Graph::hide_all_edge_ids() const {
    for (auto element : E) {
        int id = element.first;
        _gv->clearEdgeLabel(id);
    }
    update();
}

void Graph::color_reachable(Vertex* vertex) const {
    paths::breadth_first_search(vertex);

    for (auto element : V) {
        Vertex* vertex = element.second;
        if (vertex->get_path() != nullptr) {
            view_vertex_custom(vertex, COLOR_REACHABLE);
        }
    }
    
    update();
    clear();
}

void Graph::color_unreachable(Vertex* vertex) const {
    paths::breadth_first_search(vertex);

    for (auto element : V) {
        Vertex* vertex = element.second;
        if (vertex->get_path() == nullptr) {
            view_vertex_custom(vertex, COLOR_UNREACHABLE);
        }
    }

    update();
    clear();
}

void Graph::animate_path(path_t path, int ms, Color color) const {
    if (path.empty()) return;

    if (ms * path.size() > MAX_WAIT) {
        ms = std::ceil(MAX_WAIT / path.size());
    }

    for (std::size_t i = 1; i < path.size(); ++i) {
        if (i > 1 && ms > 0) sleep_for(ms);
        view_vertex_custom(path[i], color);
        view_edge_custom(path[i - 1]->edge_to(path[i]), color);
        update();
    }
}

void Graph::clear_path(path_t path, int ms) const {
    if (path.empty()) return;

    if (ms * path.size() > MAX_WAIT) {
        ms = std::ceil(MAX_WAIT / path.size());
    }

    for (std::size_t i = 1; i < path.size(); ++i) {
        if (i > 1 && ms > 0) sleep_for(ms);
        view_vertex_reset(path[i]);
        view_edge_reset(path[i - 1]->edge_to(path[i]));
        update();
    }
}

void Graph::reset_path(path_t path) const {
    if (path.empty()) return;

    for (std::size_t i = 1; i < path.size(); ++i) {
        view_vertex_reset(path[i]);
        view_edge_reset(path[i - 1]->edge_to(path[i]));
        update();
    }
}

void Graph::set_background(std::string path) const {
    _gv->setBackground(path);
}

void Graph::straight_edges(bool val) const {
    _gv->defineEdgeCurved(!val);
}

void Graph::show_boundaries() const {
    int ID = -1337;

    // Corners
    _gv->addNode(--ID, 0, 0);
    _gv->setVertexLabel(ID, "\t");
    _gv->setVertexColor(ID, COLOR_BOUNDARY);
    _gv->addNode(--ID, _width, 0);
    _gv->setVertexLabel(ID, "\t");
    _gv->setVertexColor(ID, COLOR_BOUNDARY);
    _gv->addNode(--ID, 0, _height);
    _gv->setVertexLabel(ID, "\t");
    _gv->setVertexColor(ID, COLOR_BOUNDARY);
    _gv->addNode(--ID, _width, _height);
    _gv->setVertexLabel(ID, "\t");
    _gv->setVertexColor(ID, COLOR_BOUNDARY);

    int increment;

    // Top and bottom
    increment = 200 * _width / (_width + 1000);
    for (int i = increment; i < _width - (increment / 3); i += increment) {
        _gv->addNode(--ID, i, 0);
        _gv->setVertexLabel(ID, "\t");
        _gv->setVertexColor(ID, COLOR_BOUNDARY);
        _gv->addNode(--ID, i, _height);
        _gv->setVertexLabel(ID, "\t");
        _gv->setVertexColor(ID, COLOR_BOUNDARY);
    }

    // Left and right
    increment = 200 * _height / (_height + 1000);
    for (int j = increment; j < _height - (increment / 3); j += increment) {
        _gv->addNode(--ID, 0, j);
        _gv->setVertexLabel(ID, "\t");
        _gv->setVertexColor(ID, COLOR_BOUNDARY);
        _gv->addNode(--ID, _width, j);
        _gv->setVertexLabel(ID, "\t");
        _gv->setVertexColor(ID, COLOR_BOUNDARY);
    }
}
