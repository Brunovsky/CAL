#ifndef LOADMAP_H___
#define LOADMAP_H___

#include "graph.h"

#include <string>

static const std::string meta_suffix = "_meta.txt";
static const std::string nodes_suffix = "_nodes.txt";
static const std::string roads_suffix = "_roads.txt";
static const std::string edges_suffix = "_subroads.txt";

struct metadata {
    double min_longitude, max_longitude;
    double min_latitude, max_latitude;
    double scale;
    int nodes, edges;
    int width = 0, height = 0;
    bool boundaries = false;
    bool background = false;
    bool oneway = false, bothways = false;
    bool straightedges = true;
    std::string background_filename = "";
    double density = 0.000100;
};

bool check_filename(std::string filename);

int load_map(std::string filename);

#endif // LOADMAP_H___
