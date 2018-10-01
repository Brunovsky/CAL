#include "graph.hpp"

#include <iostream>
#include <queue>
#include <stdexcept>

namespace {

void clear_vertices(Graph& graph) {
  for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
    graph[v]->visited = false;
  }
}

std::vector<edge_id> find_flow_path(Graph& graph, vertex_id source, vertex_id target) {
  clear_vertices(graph);
  graph[source]->visited = true;

  std::queue<vertex_id> vertex_queue;
  vertex_queue.push(source);

  while (!vertex_queue.empty()) {
    vertex_id current = vertex_queue.front();
    vertex_queue.pop();
    if (current == target) break;

    for (auto oe : boost::make_iterator_range(boost::out_edges(current, graph))) {
      vertex_id next = boost::target(oe, graph);

      if (graph[next]->visited || graph[oe]->flow == graph[oe]->max_flow) continue;

      graph[next]->visited = true;
      graph[next]->path = current;
      vertex_queue.push(next);
    }
  }

  // Build the path
  vertex_id cur = target;
  std::vector<edge_id> path;

  if (!graph[target]->visited) return path;

  while (cur != source) {
    edge_id edge = boost::edge(graph[cur]->path, cur, graph).first;
    path.push_back(edge);
    cur = graph[cur]->path;
  }

  std::reverse(path.begin(), path.end());

  return path;
}

void augment_path(Graph& graph, const std::vector<edge_id>& path) {
  flow_t augment = 0;

  for (edge_id e : path) {
    flow_t f = graph[e]->max_flow - graph[e]->flow;
    if (augment == 0 || f < augment) {
      augment = f;
    }
  }

  std::cout << "Augmented by: " << augment << std::endl;

  for (edge_id uv : path) {
    vertex_id u = boost::source(uv, graph);
    vertex_id v = boost::target(uv, graph);
    graph[uv]->flow += augment;

    auto vu = boost::edge(v, u, graph).first;

    graph[vu]->flow = graph[uv]->max_flow - graph[uv]->flow;
  }
}

void fill_residual(Graph& graph) {
  for (auto e : boost::make_iterator_range(boost::edges(graph))) {
    vertex_id u = boost::source(e, graph);
    vertex_id v = boost::target(e, graph);

    if (boost::edge(v, u, graph).second == false) {
      boost::add_edge(v, u, std::make_shared<Edge>(0, graph[e]->max_flow), graph);
    }

    auto re = boost::edge(v, u, graph).first;
    graph[re]->flow = graph[e]->max_flow - graph[e]->flow;
  }

}

} // anonymous namespace

void ford_fulkerson(Graph graph, vertex_id S, vertex_id T) {
  Graph residual = graph;
  fill_residual(residual);

  auto path = find_flow_path(residual, S, T);

  while (!path.empty()) {
    augment_path(residual, path);

    display_graph_flow(residual);

    path = find_flow_path(residual, S, T);
  }

  std::cout << "Done" << std::endl;

  display_graph_flow(graph, false);
}
