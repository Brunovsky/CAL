#include "loadmap.h"
#include "graph.h"

#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <regex>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#define DEBUG_LOADMAP 1

static std::unordered_map<long long, int> vertex_id_map;
static std::unordered_map<long long, int> road_id_map;

static bool string_to_bool(std::string match) {
    static const std::regex regex_true(" *true *|1", std::regex::icase);
    static const std::regex regex_false(" *false *|0", std::regex::icase);

    if (std::regex_match(match, regex_true)) {
        return true;
    }

    if (std::regex_match(match, regex_false)) {
        return false;
    }

    return static_cast<bool>(std::stoi(match));
}

static int computeX(double longitude, const metadata& meta) {
    double long_delta = meta.max_longitude - meta.min_longitude;
    return std::floor(meta.width * (longitude - meta.min_longitude) / long_delta);
}

static int computeY(double latitude, const metadata& meta) {
    double lat_delta = meta.max_latitude - meta.min_latitude;
    return std::floor(meta.height * (meta.max_latitude - latitude) / lat_delta);
}

static double deg_to_rad(double degrees) {
    static const double pi = std::acos(-1);
    return degrees * pi / 180.0;
}

static double rad_to_deg(double radians) {
    static const double pi = std::acos(-1);
    return radians * 180.0 / pi;
}

static void estimate_meta(metadata &meta) {
    static const double latitude_const = 110.574;  // 1 degree Latitude ~ 110.574 km
    static const double longitude_const = 111.320; // 1 degree Longitude ~ 111.320 * cos(latitude) km

    if (meta.width != 0 && meta.height != 0) {
        meta.width = 0; meta.height = 0;
    }

    double avg_latitude = (meta.min_latitude + meta.max_latitude) / 2.0;
    double delta_latitude = meta.max_latitude - meta.min_latitude;
    double delta_longitude = meta.max_longitude - meta.min_longitude;

    double distanceYkm = std::abs(latitude_const * delta_latitude);
    double distanceXkm = std::abs(longitude_const * delta_longitude * std::cos(deg_to_rad(avg_latitude)));

    double ratio = distanceYkm / distanceXkm;

    // In a geometry of size X x Y there are X*Y possible node positions.
    // Assume the nodes are well spread out throughout the map we are
    // going to create. If we expect node density ~ K (meta.density)
    // then the number of nodes (meta.nodes) should be approximately
    // equal to X*Y*K = X*X*K*ratio. Therefore X = sqrt(nodes / K*ratio).

    double Xd = std::sqrt(meta.nodes / (meta.density * ratio));
    double Yd = Xd * ratio;

    if (meta.width == 0 && meta.height == 0) {
        meta.width = ceil(Xd);
        meta.height = ceil(Yd);
        meta.scale = 1000.0 * distanceXkm / Xd;
    } else if (meta.width != 0 && meta.height == 0) {
        Yd = (double)(meta.width) * ratio;
        meta.height = ceil(Yd);
        meta.scale = 1000.0 * distanceYkm / Yd;
        meta.density = (double)(meta.nodes) / (Xd * Yd);
    } else if (meta.width == 0 && meta.height != 0) {
        Xd = (double)(meta.height) / ratio;
        meta.width = ceil(Xd);
        meta.scale = 1000.0 * distanceXkm / Xd;
        meta.density = (double)(meta.nodes) / (Xd * Yd);
    }
}

bool check_filename(std::string filename) {
    std::ifstream file;

    file.open(filename + meta_suffix);
    if (!file.is_open() || !file.good()) {
        std::cout << "Meta file not found (" << filename + meta_suffix << ")" << std::endl;
        return false;
    }
    file.close();

    file.open(filename + nodes_suffix);
    if (!file.is_open() || !file.good()) {
        std::cout << "Nodes file not found (" << filename + nodes_suffix << ")" << std::endl;
        return false;
    }
    file.close();

    file.open(filename + roads_suffix);
    if (!file.is_open() || !file.good()) {
        std::cout << "Roads file not found (" << filename + roads_suffix << ")" << std::endl;
        return false;
    }
    file.close();

    file.open(filename + edges_suffix);
    if (!file.is_open() || !file.good()) {
        std::cout << "Subroads file not found (" << filename + edges_suffix << ")" << std::endl;
        return false;
    }
    file.close();

    return true;
}

static int load_meta(std::string filename, metadata &meta) {
    static const std::regex reg_min_lon(R"Z(min_?longitude ?= ?(-?\d+(?:\.\d+)?)[;,])Z", std::regex::icase);
    static const std::regex reg_max_lon(R"Z(max_?longitude ?= ?(-?\d+(?:\.\d+)?)[;,])Z", std::regex::icase);
    static const std::regex reg_min_lat(R"Z(min_?latitude ?= ?(-?\d+(?:\.\d+)?)[;,])Z", std::regex::icase);
    static const std::regex reg_max_lat(R"Z(max_?latitude ?= ?(-?\d+(?:\.\d+)?)[;,])Z", std::regex::icase);
    static const std::regex reg_nodes(R"Z(nodes ?= ?(\d+)[;,])Z", std::regex::icase);
    static const std::regex reg_edges(R"Z(edges ?= ?(\d+)[;,])Z", std::regex::icase);

    static const std::regex reg_density(R"Z(density ?= ?(-?\d+(?:\.\d+)?)[;,])Z", std::regex::icase);
    static const std::regex reg_width(R"Z(width ?= ?(\d+)[;,])Z", std::regex::icase);
    static const std::regex reg_height(R"Z(height ?= ?(\d+)[;,])Z", std::regex::icase);

    static const std::regex reg_boundaries(R"Z(boundaries ?= ?([01]|(?:True|False))[;,])Z", std::regex::icase);
    static const std::regex reg_oneway(R"Z(oneway ?= ?([01]|(?:True|False))[;,])Z", std::regex::icase);
    static const std::regex reg_bothways(R"Z(bothways ?= ?([01]|(?:True|False))[;,])Z", std::regex::icase);
    static const std::regex reg_straightedges(R"Z(straight_?edges ?= ?([01]|(?:True|False))[;,])Z", std::regex::icase);
    static const std::regex reg_background(R"Z(background ?= ?(.*?)[;,])Z", std::regex::icase);

    std::ifstream file(filename);
    if (!file.is_open()) return 1;

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    file.close();

    std::smatch match;

    // Mandatory: MIN_LONGITUDE
    if (regex_search(text, match, reg_min_lon)) {
        meta.min_longitude = std::stod(match[1]);
    } else {
        return 1;
    }
    if (DEBUG_LOADMAP) std::cout << "Min longitude = " << meta.min_longitude << std::endl;

    // Mandatory: MAX_LONGITUDE
    if (regex_search(text, match, reg_max_lon)) {
        meta.max_longitude = std::stod(match[1]);
    } else {
        return 1;
    }
    if (DEBUG_LOADMAP) std::cout << "Max longitude = " << meta.max_longitude << std::endl;

    // Mandatory: MIN_LATITUDE
    if (regex_search(text, match, reg_min_lat)) {
        meta.min_latitude = std::stod(match[1]);
    } else {
        return 1;
    }
    if (DEBUG_LOADMAP) std::cout << "Min latitude = " << meta.min_latitude << std::endl;

    // Mandatory: MAX_LATITUDE
    if (regex_search(text, match, reg_max_lat)) {
        meta.max_latitude = std::stod(match[1]);
    } else {
        return 1;
    }
    if (DEBUG_LOADMAP) std::cout << "Max latitude = " << meta.max_latitude << std::endl;

    // Mandatory: NODES
    if (regex_search(text, match, reg_nodes)) {
        meta.nodes = std::stoi(match[1]);
    } else {
        return 1;
    }
    if (DEBUG_LOADMAP) std::cout << "Nodes = " << meta.nodes << std::endl;

    // Mandatory: EDGES
    if (regex_search(text, match, reg_edges)) {
        meta.edges = std::stoi(match[1]);
    } else {
        return 1;
    }
    if (DEBUG_LOADMAP) std::cout << "Edges = " << meta.edges << std::endl;

    // Optional: DENSITY
    if (regex_search(text, match, reg_density)) {
        meta.density = std::stod(match[1]);
    }
    if (DEBUG_LOADMAP) std::cout << "Density = " << meta.density << std::endl;

    // Optional: WIDTH
    if (regex_search(text, match, reg_width)) {
        meta.width = std::stoi(match[1]);
    }
    if (DEBUG_LOADMAP) std::cout << "Width = " << meta.width << std::endl;

    // Optional: HEIGHT
    if (regex_search(text, match, reg_height)) {
        meta.height = std::stoi(match[1]);
    }
    if (DEBUG_LOADMAP) std::cout << "Height = " << meta.height << std::endl;

    // Optional: BOUNDARIES
    if (regex_search(text, match, reg_boundaries)) {
        meta.boundaries = string_to_bool(match[1]);
    }
    if (DEBUG_LOADMAP) std::cout << "Boundaries = " << meta.boundaries << std::endl;

    // Optional: ONEWAY
    if (regex_search(text, match, reg_oneway)) {
        meta.oneway = string_to_bool(match[1]);
    }
    if (DEBUG_LOADMAP) std::cout << "Oneway = " << meta.oneway << std::endl;

    // Optional: BOTHWAYS
    if (regex_search(text, match, reg_bothways)) {
        meta.bothways = string_to_bool(match[1]);
    }
    if (DEBUG_LOADMAP) std::cout << "Bothways = " << meta.bothways << std::endl;

    // Optional: STRAIGHT_EDGES
    if (regex_search(text, match, reg_straightedges)) {
        meta.straightedges = string_to_bool(match[1]);
    }
    if (DEBUG_LOADMAP) std::cout << "Straightedges = " << meta.straightedges << std::endl;

    // Optional: BACKGROUND
    if (regex_search(text, match, reg_background)) {
        meta.background = true;
        meta.background_filename = match[1];
    }
    if (DEBUG_LOADMAP) std::cout << "Background = " << meta.background_filename << std::endl;

    estimate_meta(meta);
    return 0;
}

static int load_nodes(const std::string& filename, const metadata &meta) {
    static const std::regex reg(R"Z(^(\d+);(-?\d+(?:\.\d+)?);(-?\d+(?:\.\d+)?);(?:-?\d+(?:\.\d+)?);(?:-?\d+(?:\.\d+)?);?$)Z");

    std::ifstream file(filename);
    if (!file.is_open())
        return 1;

    std::string line;
    std::getline(file, line);

    int line_number = 1, count = 1;

    while (!file.eof() && !file.fail()) {
        std::smatch match;
        double latitude, longitude;

        if (std::regex_match(line, match, reg)) {
            try {
                // Get Node ID
                long long file_id = std::stoll(match[1]);
                int id = count++;
                vertex_id_map[file_id] = id;

                // Get Node Latitude
                latitude = std::stod(match[2]);
                int y = computeY(latitude, meta);

                // Get Node Longitude
                longitude = std::stod(match[3]);
                int x = computeX(longitude, meta);

                // Add Node
                Vertex* vertex = new Vertex(id, x, y);
                bool success = graph->add_vertex(vertex);

                if (!success) throw std::out_of_range("Vertex coordinates out of bounds");
                // Done
            } catch (std::exception &e) {
                std::cerr << "Error on file " << filename << std::endl;
                std::cerr << "Line: " << line_number << std::endl;
                std::cerr << e.what() << std::endl;
                std::cerr << "Vertex Latitude = " << latitude << std::endl;
                std::cerr << "Vertex Longitude = " << longitude << std::endl;
                return 2;
            }
        } else {
            std::cerr << "Line " << line_number << " format not recognised." << std::endl;
        }

        ++line_number;
        std::getline(file, line);
    }

    file.close();
    return 0;
}

static int load_roads(const std::string& filename, const metadata &meta) {
    static const std::regex reg(R"Z(^(\d+);(.*?);(false|true);?$)Z", std::regex::icase);

    std::ifstream file(filename);
    if (!file.is_open()) return 1;

    std::string line;
    std::getline(file, line);

    int line_number = 1, count = 1;

    while (!file.eof() && !file.fail()) {
        std::smatch match;

        if (std::regex_match(line, match, reg)) {
            try {
                // Get Road ID
                long long file_id = std::stoll(match[1]);
                int id = count++;
                road_id_map[file_id] = id;

                // Get Road Name
                std::string name = match[2];

                // Get Road Direction
                bool bothways;
                if (meta.bothways) {
                    bothways = true;
                } else if (meta.oneway) {
                    bothways = false;
                } else {
                    bothways = string_to_bool(match[3]);
                }

                // Register Road
                Road* road = new Road(id, name, bothways);
                graph->add_road(road);
                // Done
            } catch (std::exception &e) {
                std::cerr << "Error on file " << filename << std::endl;
                std::cerr << "Line: " << line_number << std::endl;
                std::cerr << e.what() << std::endl;
                return 2;
            }
        }

        ++line_number;
        std::getline(file, line);
    }

    file.close();
    return 0;
}

static int load_edges(const std::string& filename, const metadata &meta) {
    static const std::regex reg(R"Z(^(\d+);(\d+);(\d+);?$)Z");

    std::ifstream file(filename);
    if (!file.is_open()) return 1;

    std::string line;
    std::getline(file, line);

    int line_number = 1, count = 1;

    while (!file.eof() && !file.fail()) {
        std::smatch match;

        if (std::regex_match(line, match, reg)) {
            try {
                int roadid, vertex1id, vertex2id;

                // Get Road ID
                long long id = std::stoll(match[1]);
                if (road_id_map.count(id)) {
                    roadid = road_id_map[id];
                } else {
                    throw std::invalid_argument("Road id not found");
                }

                // Get Vertex 1
                id = std::stoll(match[2]);
                if (vertex_id_map.count(id)) {
                    vertex1id = vertex_id_map[id];
                } else {
                    throw std::invalid_argument("Source vertex id not found");
                }

                // Get Vertex 2
                id = std::stoll(match[3]);
                if (vertex_id_map.count(id)) {
                    vertex2id = vertex_id_map[id];
                } else {
                    throw std::invalid_argument("Target vertex id not found");
                }

                // Get vertices and distance
                Vertex* source = graph->get_vertex(vertex1id);
                Vertex* target = graph->get_vertex(vertex2id);
                Road* road = graph->get_road(roadid);

                // Load edge
                Edge* edge = new Edge(count++, source, target, road);
                graph->add_edge(edge);
                road->add_edge(edge, true);

                if (road->bothways()) {
                    edge = new Edge(count++, target, source, road);
                    graph->add_edge(edge);
                    road->add_edge(edge, false);
                }
                // Done
            } catch (std::exception &e) {
                std::cerr << "Error on file " << filename << std::endl;
                std::cerr << "Line: " << line_number << std::endl;
                std::cerr << e.what() << std::endl;
                return 2;
            }
        }

        ++line_number;
        std::getline(file, line);
    }

    file.close();
    return 0;
}

int load_map(std::string filename) {
    if (!check_filename(filename)) {
        return 1;
    }

    metadata meta;
    if (load_meta(filename + meta_suffix, meta) != 0) {
        return 1;
    }

    graph = std::make_unique<Graph>(meta.width, meta.height, meta.scale);

    if (meta.boundaries) graph->show_boundaries();

    if (meta.background) graph->set_background(meta.background_filename);

    if (meta.straightedges) graph->straight_edges();

    if (load_nodes(filename + nodes_suffix, meta) != 0) {
        return 1;
    }

    if (load_roads(filename + roads_suffix, meta) != 0) {
        return 1;
    }

    if (load_edges(filename + edges_suffix, meta) != 0) {
        return 1;
    }

    graph->update();
    return 0;
}
