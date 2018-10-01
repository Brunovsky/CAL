#ifndef GRAPH_H___
#define GRAPH_H___

#include "graphviewer.h"
#include "MutablePriorityQueue.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

#define VIEW_WIDTH_DEFAULT              ((int)600)
#define VIEW_HEIGHT_DEFAULT             ((int)600)
#define VIEW_VERTEX_SIZE_CLEAR          ((int)0)
#define VIEW_VERTEX_SIZE_ACCIDENTED     ((int)8)
#define VIEW_VERTEX_SIZE_CUSTOM         ((int)10)
#define VIEW_VERTEX_SIZE_SELECTED       ((int)20)
#define VIEW_EDGE_THICKNESS_CLEAR       ((int)1)
#define VIEW_EDGE_THICKNESS_ACCIDENTED  ((int)2)
#define VIEW_EDGE_THICKNESS_CUSTOM      ((int)3)
#define VIEW_EDGE_THICKNESS_SELECTED    ((int)4)

#define COLOR_CLEAR                   BLUE
#define COLOR_ACCIDENTED              RED
#define COLOR_SELECTED                MAGENTA
#define COLOR_REACHABLE               BLUE
#define COLOR_UNREACHABLE             YELLOW
#define COLOR_BOUNDARY                BLACK

#define PATH_COLOR_1                  GREEN
#define PATH_COLOR_2                  CYAN
#define PATH_COLOR_3                  ORANGE

#define MAX_WAIT                      10000.0

class Graph;
class Vertex;
class Edge;
class Road;

extern std::unique_ptr<Graph> graph; // Singleton graph instance

using Color = std::string;

using path_t = std::vector<Vertex*>;

class Graph {
protected:
    GraphViewer* const _gv;
    const int _width;
    const int _height;
    const double _scale;
    std::unordered_map<int, Vertex*> V;
    std::unordered_map<int, Edge*> E;
    std::unordered_map<int, Road*> R;

    bool within_bounds(int x, int y) const;

public:
    explicit Graph(int width, int height, double scale);
    ~Graph();

    // ***** GraphViewer CRUD
    void update() const;
    void reset() const;

    void view_vertex_custom(Vertex* vertex, Color color) const;
    void view_vertex_select(Vertex* vertex) const;
    void view_vertex_reset(Vertex* vertex) const;
    void view_vertex_label(Vertex* vertex, int label) const;

    void view_edge_custom(Edge* edge, Color color) const;
    void view_edge_select(Edge* edge) const;
    void view_edge_reset(Edge* edge) const;
    void view_edge_label(Edge* edge, int label) const;

    void show_all_vertex_ids() const;
    void hide_all_vertex_ids() const;
    void show_all_edge_ids() const;
    void hide_all_edge_ids() const;
    void view_road(Road* road, int label) const;

    void show_boundaries() const;
    void set_background(std::string path) const;
    void straight_edges(bool val = true) const;
    // *****

    // ***** Self CRUD
    void clear() const;

    void color_reachable(Vertex* vertex) const;
    void color_unreachable(Vertex* vertex) const;

    void animate_path(path_t path, int ms, Color color) const;
    void clear_path(path_t path, int ms) const;
    void reset_path(path_t path) const;
    // *****

    // ***** Vertex CRUD
    bool add_vertex(Vertex* vertex);

    double distance(Vertex* v1, Vertex* v2) const;

    Vertex* get_vertex(int vid) const;
    // *****

    // ***** Edge CRUD
    bool add_edge(Edge* edge);

    double length(Edge* edge) const;

    Edge* get_edge(int eid) const;
    Edge* get_edge(int vsource, int vtarget) const;
    // *****
    
    // ***** Road CRUD
    bool add_road(Road* road);

    Road* get_road(int rid) const;
    const std::unordered_map<int, Road*>& get_road_map() const;
    // *****
    
    void regenerate();
    
    friend class Vertex;
    friend class Edge;
    friend class Road;
};



class Vertex {
protected:
    const int _id;
    const int _x, _y;

    std::unordered_set<Edge*> _in;
    std::unordered_set<Edge*> _out;
    std::unordered_set<Edge*> _acc_in;
    std::unordered_set<Edge*> _acc_out;

    bool _accidented = false;
    Vertex* _path = nullptr;
    double _priority = 0, _cost = 0;
    int queueIndex = 0;
    
    Graph* _graph = nullptr;

public:
    explicit Vertex(int id, int x, int y);

    // ***** Self CRUD
    int id() const;
    int x() const;
    int y() const;

    bool is_accidented() const;
    bool is_clear() const;

    bool fix();
    bool accident();
    // *****
    
    // ***** Edge CRUD
    Edge* edge_to(Vertex* other) const;
    Edge* edge_from(Vertex* other) const;

    bool connects_to(Vertex* other) const;
    bool connects_from(Vertex* other) const;

    std::size_t in_degree() const;
    std::size_t out_degree() const;

    const std::unordered_set<Edge*>& incident() const;
    const std::unordered_set<Edge*>& outgoing() const;
    // *****

    // ***** Algorithms
    double distance(Vertex* other) const;

    void set_path(Vertex* previous);
    void set_priority(double priority);
    void set_cost(double cost);
    
    Vertex* get_path() const;
    double get_priority() const;
    double get_cost() const;
    // *****

    friend class Graph;
    friend class Edge;
    friend class Road;
    friend class MutablePriorityQueue<Vertex>;
    friend bool operator<(const Vertex& v1, const Vertex& v2);
};



class Edge {
protected:
    const int _id;
    Vertex* const _source;
    Vertex* const _target;

    bool _accidented = false;
    double _weight = 0;

    Road* _road = nullptr;

    Graph* _graph = nullptr;

public:
    explicit Edge(int id, Vertex* source, Vertex* target, Road* road);

    // ***** Self CRUD
    int id() const;
    Vertex* source() const;
    Vertex* target() const;
    Road* road() const;

    bool is_accidented() const;
    bool is_clear() const;

    bool fix();
    bool accident();
    // *****
    
    // ***** Algorithms
    double length() const;

    void set_weight(double weight);

    double get_weight() const;
    // *****
    
    friend class Graph;
    friend class Vertex;
    friend class Road;
};



class Road {
protected:
    const int _id;
    const std::string _name;
    const bool _bothways;

    std::vector<Edge*> _forward;
    std::vector<Edge*> _backward;

    Graph* _graph = nullptr;

public:
    explicit Road(int id, std::string name, bool bothways = false);

    // ***** Self CRUD
    int id() const;
    const std::string& name() const;
    bool bothways() const;

    double total_length() const;
    bool is_accidented(bool forward = true) const;
    bool is_clear(bool forward = true) const;
    // *****
    
    // ***** Vertex/Edge CRUD
    bool add_edge(Edge* edge, bool forward = true);
    Vertex* start_vertex() const;
    Vertex* end_vertex() const;
    Edge* first_edge() const;
    Edge* last_edge() const;
    // *****
    
    friend class Graph;
    friend class Edge;
};

#endif // GRAPH_H___
