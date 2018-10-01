#include "graph.hpp"

#include <boost/heap/pairing_heap.hpp>

#include <unordered_map>
#include <iostream>

namespace {

void clear_vertices(const Graph& graph) {
  for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
    graph[v]->visited = false;
    graph[v]->priority = 0;
    graph[v]->cost = 0;
    graph[v]->select = false;
  }
}

bool dijkstra_distance(Graph& graph, vertex_id source, vertex_id target) {
  using namespace boost::heap;

  clear_vertices(graph);

  auto compare_vertex = [&](const vertex_id& v1, const vertex_id& v2) {
    return graph[v1]->priority > graph[v2]->priority;
  };

  using queue_t = pairing_heap<vertex_id, compare<decltype(compare_vertex)>>;

  queue_t queue(compare_vertex);
  std::unordered_map<vertex_id, queue_t::handle_type> handle_map;

  handle_map[source] = queue.push(source);

  while (!queue.empty()) {
    vertex_id current = queue.top();
    queue.pop();

    if (current == target) break;

    for (auto oe : boost::make_iterator_range(out_edges(current, graph))) {
      vertex_id next = boost::target(oe, graph);

      cost_t cost = graph[current]->cost + distance(*graph[current], *graph[next]);

      std::cout << "Cost: " << cost << " | ";
      std::cout << current << "  " << next << std::endl;

      if (graph[next]->visited && cost < graph[next]->cost) {
        graph[next]->cost = cost;
        graph[next]->priority = cost;
        graph[next]->path = current;
        queue.update(handle_map[next]);
      } else if (!graph[next]->visited) {
        graph[next]->cost = cost;
        graph[next]->priority = cost;
        graph[next]->path = current;
        graph[next]->visited = true;
        handle_map[next] = queue.push(next);
      }
    }
  }

  if (graph[target]->visited) {
    vertex_id cur = target;
    graph[cur]->select = true;

    while (cur != source) {
      vertex_id next = cur;
      cur = graph[next]->path;
      graph[cur]->select = true;
      graph[boost::edge(cur, next, graph).first]->select = true;
    }

    return true;
  }

  return false;
}

bool dijkstra_weight(Graph& graph, vertex_id source, vertex_id target) {
  using namespace boost::heap;

  clear_vertices(graph);

  auto compare_vertex = [&](const vertex_id& v1, const vertex_id& v2) {
    return graph[v1]->priority > graph[v2]->priority;
  };

  using queue_t = pairing_heap<vertex_id, compare<decltype(compare_vertex)>>;

  queue_t queue(compare_vertex);
  std::unordered_map<vertex_id, queue_t::handle_type> handle_map;

  handle_map[source] = queue.push(source);

  while (!queue.empty()) {
    vertex_id current = queue.top();
    queue.pop();

    if (current == target) break;

    for (auto oe : boost::make_iterator_range(out_edges(current, graph))) {
      vertex_id next = boost::target(oe, graph);

      cost_t cost = graph[current]->cost + graph[oe]->weight;

      std::cout << "Cost: " << cost << " | ";
      std::cout << current << "  " << next << std::endl;

      if (graph[next]->visited && cost < graph[next]->cost) {
        graph[next]->cost = cost;
        graph[next]->priority = cost;
        graph[next]->path = current;
        queue.update(handle_map[next]);
      } else if (!graph[next]->visited) {
        graph[next]->cost = cost;
        graph[next]->priority = cost;
        graph[next]->path = current;
        graph[next]->visited = true;
        handle_map[next] = queue.push(next);
      }
    }
  }

  if (graph[target]->visited) {
    vertex_id cur = target;
    graph[cur]->select = true;

    while (cur != source) {
      vertex_id next = cur;
      cur = graph[next]->path;
      graph[cur]->select = true;
      graph[boost::edge(cur, next, graph).first]->select = true;
    }

    return true;
  }
  
  return false;
}

bool astar_distance(Graph& graph, vertex_id source, vertex_id target) {
  using namespace boost::heap;

  clear_vertices(graph);

  auto compare_vertex = [&](const vertex_id& v1, const vertex_id& v2) {
    return graph[v1]->priority > graph[v2]->priority;
  };

  using queue_t = pairing_heap<vertex_id, compare<decltype(compare_vertex)>>;

  queue_t queue(compare_vertex);
  std::unordered_map<vertex_id, queue_t::handle_type> handle_map;

  handle_map[source] = queue.push(source);

  while (!queue.empty()) {
    vertex_id current = queue.top();
    queue.pop();

    if (current == target) break;

    for (auto oe : boost::make_iterator_range(out_edges(current, graph))) {
      vertex_id next = boost::target(oe, graph);

      cost_t cost = graph[current]->cost + distance(*graph[current], *graph[next]);

      std::cout << "Cost: " << cost << " | ";
      std::cout << current << "  " << next << std::endl;

      if (graph[next]->visited && cost < graph[next]->cost) {
        graph[next]->cost = cost;
        graph[next]->priority = cost + distance(*graph[next], *graph[target]);
        graph[next]->path = current;
        queue.update(handle_map[next]);
      } else if (!graph[next]->visited) {
        graph[next]->cost = cost;
        graph[next]->priority = cost + distance(*graph[next], *graph[target]);
        graph[next]->path = current;
        graph[next]->visited = true;
        handle_map[next] = queue.push(next);
      }
    }
  }

  if (graph[target]->visited) {
    vertex_id cur = target;
    graph[cur]->select = true;

    while (cur != source) {
      vertex_id next = cur;
      cur = graph[next]->path;
      graph[cur]->select = true;
      graph[boost::edge(cur, next, graph).first]->select = true;
    }

    return true;
  }

  return false;
}

}

void dijkstra_weight_path(Graph graph, vertex_id source, vertex_id target) {
  display_graph_weight(graph);

  if (!dijkstra_weight(graph, source, target)) {
    std::cout << "No path exists." << std::endl;
  }

  display_graph_weight(graph, false);
}

void dijkstra_distance_path(Graph graph, vertex_id source, vertex_id target) {
  display_graph_distance(graph);

  if (!dijkstra_distance(graph, source, target)) {
    std::cout << "No path exists." << std::endl;
  }

  display_graph_distance(graph, false);
}

void astar_distance_path(Graph graph, vertex_id source, vertex_id target) {
  display_graph_distance(graph);

  if (!astar_distance(graph, source, target)) {
    std::cout << "No path exists" << std::endl;
  }

  display_graph_distance(graph, false);
}
