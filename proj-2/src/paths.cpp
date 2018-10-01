#include "paths.h"
#include "MutablePriorityQueue.h"

#include <queue>
#include <algorithm>

namespace paths {

/**
 * Returns the path found by an algorithm from source to target.
 */
path_t get_path(Vertex* source, Vertex* target) {
    Vertex* current = target;

    path_t path;

    do {
        path.push_back(current);
        current = current->get_path();
    } while (current != source && current != nullptr);

    if (current == nullptr) {
        return path_t();
    } else {
        path.push_back(source);
        std::reverse(path.begin(), path.end());
        return path;
    }
}

/**
 * Breadth-First search.
 */
void breadth_first_search(Vertex* source) {
    std::queue<Vertex*> vertex_queue;

    vertex_queue.push(source);

    while (!vertex_queue.empty()) {
        Vertex* current = vertex_queue.front();
        vertex_queue.pop();

        for (Edge* edge : current->outgoing()) {
            Vertex* next = edge->target();

            if (!next->get_path()) {
                next->set_path(current);
                vertex_queue.push(next);
            }
        }
    }
}

/**
 * Greedy Best-First Search
 */
void greedy_best_first_search(Vertex* source, Vertex* target) {
    MutablePriorityQueue<Vertex> vertex_queue;

    vertex_queue.insert(source);

    while (!vertex_queue.empty()) {
        Vertex* current = vertex_queue.extractMin();
        if (current == target) break;

        for (Edge* edge : current->outgoing()) {
            Vertex* next = edge->target();

            if (next->get_path() != nullptr) continue;

            next->set_priority(next->distance(current));

            next->set_path(current);

            vertex_queue.insert(next);
        }
    }
}

/**
 * Dijkstra late exit
 */
void dijkstra_late_exit(Vertex* source, Vertex* target) {
    MutablePriorityQueue<Vertex> vertex_queue;

    vertex_queue.insert(source);

    while (!vertex_queue.empty()) {
        Vertex* current = vertex_queue.extractMin();
        //if (current == target) break;

        for (Edge* edge : current->outgoing()) {
            Vertex* next = edge->target();

            auto newcost = current->get_cost() + next->distance(current);

            if (next->get_path() == nullptr) {
                next->set_cost(newcost);
                next->set_priority(newcost);
                next->set_path(current);
                vertex_queue.insert(next);
            } else if (newcost < next->get_cost()) {
                next->set_cost(newcost);
                next->set_priority(newcost);
                next->set_path(current);
                vertex_queue.decreaseKey(next);
            }
        }
    }
}

/**
 * Dijkstra early exit
 */
void dijkstra_early_exit(Vertex* source, Vertex* target) {
    MutablePriorityQueue<Vertex> vertex_queue;

    vertex_queue.insert(source);

    while (!vertex_queue.empty()) {
        Vertex* current = vertex_queue.extractMin();
        if (current == target) break;

        for (Edge* edge : current->outgoing()) {
            Vertex* next = edge->target();

            auto newcost = current->get_cost() + next->distance(current);

            if (next->get_path() == nullptr) {
                next->set_cost(newcost);
                next->set_priority(newcost);
                next->set_path(current);
                vertex_queue.insert(next);
            } else if (newcost < next->get_cost()) {
                next->set_cost(newcost);
                next->set_priority(newcost);
                next->set_path(current);
                vertex_queue.decreaseKey(next);
            }
        }
    }
}

/**
 * A*
 */
void astar_search(Vertex* source, Vertex* target) {
    MutablePriorityQueue<Vertex> vertex_queue;

    vertex_queue.insert(source);

    while (!vertex_queue.empty()) {
        Vertex* current = vertex_queue.extractMin();
        if (current == target) break;

        for (Edge* edge : current->outgoing()) {
            Vertex* next = edge->target();

            auto newcost = current->get_cost() + next->distance(current);

            if (next->get_path() == nullptr) {
                next->set_cost(newcost);
                next->set_priority(newcost + target->distance(next));
                next->set_path(current);
                vertex_queue.insert(next);
            } else if (newcost < next->get_cost()) {
                next->set_cost(newcost);
                next->set_priority(newcost + target->distance(next));
                next->set_path(current);
                vertex_queue.decreaseKey(next);
            }
        }
    }
}

/**
 * Dijkstra weighted
 */
void dijkstra_weight(Vertex* source, Vertex* target) {
    MutablePriorityQueue<Vertex> vertex_queue;

    vertex_queue.insert(source);

    while (!vertex_queue.empty()) {
        Vertex* current = vertex_queue.extractMin();
        if (current == target) break;

        for (Edge* edge : current->outgoing()) {
            Vertex* next = edge->target();

            auto newcost = current->get_cost() + edge->get_weight();

            if (next->get_path() == nullptr) {
                next->set_cost(newcost);
                next->set_priority(newcost);
                next->set_path(current);
                vertex_queue.insert(next);
            } else if (newcost < next->get_cost()) {
                next->set_cost(newcost);
                next->set_priority(newcost);
                next->set_path(current);
                vertex_queue.decreaseKey(next);
            }
        }
    }
}

}
