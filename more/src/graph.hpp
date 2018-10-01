#ifndef GRAPH_HPP___
#define GRAPH_HPP___

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <memory>
#include <type_traits>

using GraphTraits = boost::adjacency_list_traits<boost::setS, boost::vecS, boost::directedS>;
using vertex_id = GraphTraits::vertex_descriptor;
using edge_id = GraphTraits::edge_descriptor;

using distance_t = double;
using flow_t = unsigned;
using weight_t = distance_t;
using priority_t = double;
using cost_t = weight_t;

struct Vertex {
  const int x, y;

  vertex_id path;

  cost_t cost = 0;
  priority_t priority = 0;

  bool visited = false;
  bool select = false;

  explicit Vertex(int x, int y);
};

struct Edge {
  weight_t weight = 0;

  flow_t flow = 0;
  flow_t max_flow = 0;

  bool select = false;

  explicit Edge(flow_t flow, flow_t max_flow);
  explicit Edge(weight_t weight);
};

double distance(const Vertex& v1, const Vertex& v2);

using Graph = boost::adjacency_list<
  boost::setS,
  boost::vecS,
  boost::directedS,
  std::shared_ptr<Vertex>,
  std::shared_ptr<Edge>
>;

static_assert(std::is_same<vertex_id, Graph::vertex_descriptor>::value);
static_assert(std::is_same<edge_id, Graph::edge_descriptor>::value);

using vertex_iter = Graph::vertex_iterator;
using edge_iter = Graph::edge_iterator;
using out_edge_iter = Graph::out_edge_iterator;
using in_edge_iter = Graph::in_edge_iterator;
using adj_iter = Graph::adjacency_iterator;
using inv_adj_iter = Graph::inv_adjacency_iterator;
using vsize_t = Graph::vertices_size_type;
using esize_t = Graph::edges_size_type;
using dsize_t = Graph::degree_size_type;

void display_something();

void display_graph_implementation();

void display_graph_flow(const Graph& graph, bool hide = true);

void display_graph_weight(const Graph& graph, bool hide = true);

void display_graph_distance(const Graph& graph, bool hide = true);

void ford_fulkerson(Graph graph, vertex_id S, vertex_id T);

void dijkstra_weight_path(Graph graph, vertex_id S, vertex_id T);

void dijkstra_distance_path(Graph graph, vertex_id source, vertex_id target);

void astar_distance_path(Graph graph, vertex_id source, vertex_id target);

#endif // GRAPH_HPP___
