#include "graph.hpp"
#include "graphviewer.h"

#include <string>
#include <iostream>

#define GVW_WIDTH  600
#define GVW_HEIGHT 600

void display_graph_implementation() {
  std::cout << "  " << typeid(vertex_id).name() << std::endl;
  std::cout << "  " << typeid(edge_id).name() << std::endl;
  std::cout << "  " << typeid(vsize_t).name() << std::endl;
  std::cout << "  " << typeid(esize_t).name() << std::endl;
  std::cout << "  " << typeid(dsize_t).name() << std::endl;
  std::cout << "  " << typeid(vertex_iter).name() << std::endl;
  std::cout << "  " << typeid(edge_iter).name() << std::endl;
  std::cout << "  " << typeid(out_edge_iter).name() << std::endl;
  std::cout << "  " << typeid(in_edge_iter).name() << std::endl;
  std::cout << "  " << typeid(adj_iter).name() << std::endl;
  std::cout << "  " << typeid(inv_adj_iter).name() << std::endl;
}

void display_graph_flow(const Graph& graph, bool hide) {
  auto gv = std::make_unique<GraphViewer>(GVW_WIDTH, GVW_HEIGHT, false);
  gv->createWindow(GVW_WIDTH, GVW_HEIGHT);
  gv->defineEdgeCurved(true);

  for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
    gv->addNode(v, graph[v]->x, graph[v]->y);
    gv->setVertexLabel(v, std::to_string(v));
    if (graph[v]->select) gv->setVertexColor(v, RED);
  }

  int id = 0;
  for (auto e : boost::make_iterator_range(boost::edges(graph))) {
    auto src = boost::source(e, graph);
    auto tar = boost::target(e, graph);
    gv->addEdge(id, src, tar);
    //gv->setEdgeLabel(id, std::to_string(id));
    gv->setEdgeWeight(id, graph[e]->max_flow);
    gv->setEdgeFlow(id, graph[e]->flow);
    if (graph[e]->select) gv->setEdgeColor(id, RED);
    ++id;
  }

  gv->rearrange();

  std::string s;
  std::getline(std::cin, s);
  if (hide) gv->closeWindow();
}

void display_graph_weight(const Graph& graph, bool hide) {
  auto gv = std::make_unique<GraphViewer>(GVW_WIDTH, GVW_HEIGHT, false);
  gv->createWindow(GVW_WIDTH, GVW_HEIGHT);
  gv->defineEdgeCurved(true);

  for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
    gv->addNode(v, graph[v]->x, graph[v]->y);
    gv->setVertexLabel(v, std::to_string(v));
    if (graph[v]->select) gv->setVertexColor(v, RED);
  }

  int id = 0;
  for (auto e : boost::make_iterator_range(boost::edges(graph))) {
    auto src = boost::source(e, graph);
    auto tar = boost::target(e, graph);
    gv->addEdge(id, src, tar);
    //gv->setEdgeLabel(id, std::to_string(id));
    gv->setEdgeWeight(id, graph[e]->weight);
    if (graph[e]->select) gv->setEdgeColor(id, RED);
    ++id;
  }

  gv->rearrange();

  std::string s;
  std::getline(std::cin, s);
  if (hide) gv->closeWindow();
}

void display_graph_distance(const Graph& graph, bool hide) {
  auto gv = std::make_unique<GraphViewer>(GVW_WIDTH, GVW_HEIGHT, false);
  gv->createWindow(GVW_WIDTH, GVW_HEIGHT);
  gv->defineEdgeCurved(true);

  for (auto v : boost::make_iterator_range(boost::vertices(graph))) {
    gv->addNode(v, graph[v]->x, graph[v]->y);
    gv->setVertexLabel(v, std::to_string(v));
    if (graph[v]->select) gv->setVertexColor(v, RED);
  }

  int id = 0;
  for (auto e : boost::make_iterator_range(boost::edges(graph))) {
    auto src = boost::source(e, graph);
    auto tar = boost::target(e, graph);
    gv->addEdge(id, src, tar);
    //gv->setEdgeLabel(id, std::to_string(id));
    gv->setEdgeWeight(id, distance(*graph[src], *graph[tar]));
    if (graph[e]->select) gv->setEdgeColor(id, RED);
    ++id;
  }

  gv->rearrange();

  std::string s;
  std::getline(std::cin, s);
  if (hide) gv->closeWindow();
}
