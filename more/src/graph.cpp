#include "graph.hpp"

#include <numeric>

Vertex::Vertex(int x, int y) :
  x(x), y(y) {}

Edge::Edge(flow_t flow, flow_t max_flow) :
  flow(flow), max_flow(max_flow) {}

Edge::Edge(weight_t weight) :
  weight(weight) {}

double distance(const Vertex& v1, const Vertex& v2) {
  return std::hypot(v1.x - v2.x, v1.y - v2.y);
}
