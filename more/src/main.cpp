#include "graph.hpp"
#include "algo.hpp"

namespace {

namespace FV1 {
  vertex_id S = 0;
  vertex_id A = 1;
  vertex_id B = 2;
  vertex_id C = 3;
  vertex_id D = 4;
  vertex_id T = 5;
}

Graph create_fulkerson1() {
  using namespace FV1;
  Graph graph;

  // S
  boost::add_vertex(std::make_shared<Vertex>(300,  50), graph);
  // A
  boost::add_vertex(std::make_shared<Vertex>(200, 150), graph);
  // B
  boost::add_vertex(std::make_shared<Vertex>(400, 150), graph);
  // C
  boost::add_vertex(std::make_shared<Vertex>(200, 250), graph);
  // D
  boost::add_vertex(std::make_shared<Vertex>(400, 250), graph);
  // T
  boost::add_vertex(std::make_shared<Vertex>(300, 350), graph);

  boost::add_edge(S, A, std::make_shared<Edge>(0, 3), graph);
  boost::add_edge(S, B, std::make_shared<Edge>(0, 2), graph);
  boost::add_edge(A, B, std::make_shared<Edge>(0, 1), graph);
  boost::add_edge(A, C, std::make_shared<Edge>(0, 3), graph);
  boost::add_edge(A, D, std::make_shared<Edge>(0, 4), graph);
  boost::add_edge(B, D, std::make_shared<Edge>(0, 2), graph);
  boost::add_edge(C, T, std::make_shared<Edge>(0, 2), graph);
  boost::add_edge(D, T, std::make_shared<Edge>(0, 3), graph);

  return graph;
}

namespace DV1 {
  vertex_id S = 0;
  vertex_id A = 1;
  vertex_id B = 2;
  vertex_id C = 3;
  vertex_id D = 4;
  vertex_id E = 5;
  vertex_id T = 6;
}

Graph create_distance1() {
  using namespace DV1;
  Graph graph;

  // S
  boost::add_vertex(std::make_shared<Vertex>( 50,  50), graph);
  // A
  boost::add_vertex(std::make_shared<Vertex>(260,  50), graph);
  // B
  boost::add_vertex(std::make_shared<Vertex>(120, 250), graph);
  // C
  boost::add_vertex(std::make_shared<Vertex>( 90, 520), graph);
  // D
  boost::add_vertex(std::make_shared<Vertex>(400, 200), graph);
  // E
  boost::add_vertex(std::make_shared<Vertex>(550, 350), graph);
  // T
  boost::add_vertex(std::make_shared<Vertex>(530, 530), graph);

  boost::add_edge(S, A, std::make_shared<Edge>(0), graph);
  boost::add_edge(S, B, std::make_shared<Edge>(0), graph);
  boost::add_edge(A, D, std::make_shared<Edge>(0), graph);
  boost::add_edge(B, C, std::make_shared<Edge>(0), graph);
  boost::add_edge(B, D, std::make_shared<Edge>(0), graph);
  boost::add_edge(C, T, std::make_shared<Edge>(0), graph);
  boost::add_edge(D, A, std::make_shared<Edge>(0), graph);
  boost::add_edge(D, E, std::make_shared<Edge>(0), graph);
  boost::add_edge(D, T, std::make_shared<Edge>(0), graph);
  boost::add_edge(E, D, std::make_shared<Edge>(0), graph);
  boost::add_edge(E, T, std::make_shared<Edge>(0), graph);
  boost::add_edge(T, B, std::make_shared<Edge>(0), graph);

  return graph;
}

namespace WV1 {
  vertex_id A = 0;
  vertex_id B = 1;
  vertex_id C = 2;
  vertex_id D = 3;
  vertex_id E = 4;
  vertex_id F = 5;
  vertex_id G = 6;
  vertex_id H = 7;
  vertex_id I = 8;
}

Graph create_weight1() {
  using namespace WV1;
  Graph graph;

  // A
  boost::add_vertex(std::make_shared<Vertex>(100, 100), graph);
  // B
  boost::add_vertex(std::make_shared<Vertex>(300, 100), graph);
  // C
  boost::add_vertex(std::make_shared<Vertex>(500, 100), graph);
  // D
  boost::add_vertex(std::make_shared<Vertex>(100, 300), graph);
  // E
  boost::add_vertex(std::make_shared<Vertex>(300, 300), graph);
  // F
  boost::add_vertex(std::make_shared<Vertex>(500, 300), graph);
  // G
  boost::add_vertex(std::make_shared<Vertex>(100, 500), graph);
  // H
  boost::add_vertex(std::make_shared<Vertex>(300, 500), graph);
  // I
  boost::add_vertex(std::make_shared<Vertex>(500, 500), graph);

  boost::add_edge(A, B, std::make_shared<Edge>(300), graph);
  boost::add_edge(A, D, std::make_shared<Edge>(100), graph);
  boost::add_edge(B, C, std::make_shared<Edge>(150), graph);
  boost::add_edge(B, E, std::make_shared<Edge>( 50), graph);
  boost::add_edge(C, F, std::make_shared<Edge>(200), graph);
  boost::add_edge(D, E, std::make_shared<Edge>( 50), graph);
  boost::add_edge(D, G, std::make_shared<Edge>(200), graph);
  boost::add_edge(E, C, std::make_shared<Edge>( 50), graph);
  boost::add_edge(E, F, std::make_shared<Edge>(300), graph);
  boost::add_edge(E, G, std::make_shared<Edge>(100), graph);
  boost::add_edge(E, H, std::make_shared<Edge>(160), graph);
  boost::add_edge(F, I, std::make_shared<Edge>( 50), graph);
  boost::add_edge(G, H, std::make_shared<Edge>(100), graph);
  boost::add_edge(H, I, std::make_shared<Edge>(150), graph);

  return graph;
}

namespace WV2 {
  vertex_id A = 0;
  vertex_id B = 1;
  vertex_id C = 2;
  vertex_id D = 3;
  vertex_id E = 4;
  vertex_id F = 5;
  vertex_id G = 6;
}

Graph create_weight2() {
  using namespace WV2;
  Graph graph;

  // A
  boost::add_vertex(std::make_shared<Vertex>( 50, 200), graph);
  // B
  boost::add_vertex(std::make_shared<Vertex>( 50, 400), graph);
  // C
  boost::add_vertex(std::make_shared<Vertex>(250, 200), graph);
  // D
  boost::add_vertex(std::make_shared<Vertex>(200, 400), graph);
  // E
  boost::add_vertex(std::make_shared<Vertex>(550, 200), graph);
  // F
  boost::add_vertex(std::make_shared<Vertex>(400, 400), graph);
  // G
  boost::add_vertex(std::make_shared<Vertex>(550, 400), graph);

  boost::add_edge(A, C, std::make_shared<Edge>(5), graph);
  boost::add_edge(C, E, std::make_shared<Edge>(6), graph);
  boost::add_edge(A, B, std::make_shared<Edge>(3), graph);
  boost::add_edge(A, D, std::make_shared<Edge>(6), graph);
  boost::add_edge(C, D, std::make_shared<Edge>(2), graph);
  boost::add_edge(C, F, std::make_shared<Edge>(3), graph);
  boost::add_edge(C, G, std::make_shared<Edge>(7), graph);
  boost::add_edge(E, F, std::make_shared<Edge>(5), graph);
  boost::add_edge(E, G, std::make_shared<Edge>(2), graph);
  boost::add_edge(B, D, std::make_shared<Edge>(2), graph);
  boost::add_edge(D, F, std::make_shared<Edge>(9), graph);
  boost::add_edge(F, G, std::make_shared<Edge>(1), graph);

  return graph;
}

namespace WV3 {
  vertex_id A = 0;
  vertex_id B = 1;
  vertex_id C = 2;
  vertex_id D = 3;
  vertex_id E = 4;
  vertex_id F = 5;
  vertex_id G = 6;
  vertex_id H = 7;
}

Graph create_weight3() {
  using namespace WV3;
  Graph graph;

  // A
  boost::add_vertex(std::make_shared<Vertex>( 50, 280), graph);
  // B
  boost::add_vertex(std::make_shared<Vertex>(170,  80), graph);
  // C
  boost::add_vertex(std::make_shared<Vertex>(150, 500), graph);
  // D
  boost::add_vertex(std::make_shared<Vertex>(200, 300), graph);
  // E
  boost::add_vertex(std::make_shared<Vertex>(350, 500), graph);
  // F
  boost::add_vertex(std::make_shared<Vertex>(490, 100), graph);
  // G
  boost::add_vertex(std::make_shared<Vertex>(450, 450), graph);
  // H
  boost::add_vertex(std::make_shared<Vertex>(550, 250), graph);

  boost::add_edge(A, B, std::make_shared<Edge>(8), graph);
  boost::add_edge(A, C, std::make_shared<Edge>(2), graph);
  boost::add_edge(A, D, std::make_shared<Edge>(5), graph);
  boost::add_edge(B, D, std::make_shared<Edge>(2), graph);
  boost::add_edge(B, F, std::make_shared<Edge>(13), graph);
  boost::add_edge(C, D, std::make_shared<Edge>(2), graph);
  boost::add_edge(C, E, std::make_shared<Edge>(5), graph);
  boost::add_edge(D, E, std::make_shared<Edge>(1), graph);
  boost::add_edge(D, F, std::make_shared<Edge>(6), graph);
  boost::add_edge(D, G, std::make_shared<Edge>(3), graph);
  boost::add_edge(E, G, std::make_shared<Edge>(1), graph);
  boost::add_edge(F, G, std::make_shared<Edge>(2), graph);
  boost::add_edge(F, H, std::make_shared<Edge>(3), graph);
  boost::add_edge(G, F, std::make_shared<Edge>(2), graph);
  boost::add_edge(G, H, std::make_shared<Edge>(6), graph);

  return graph;
}

}

static std::vector<int> seq {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

int main() {
}