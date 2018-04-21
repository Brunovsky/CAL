/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <limits>
#include <cmath>
#include <iostream>

using namespace std;

template <class T> class Edge;
template <class T> class Vertex;
template <class T> class Graph;

constexpr auto INF = std::numeric_limits<double>::max();

/*
 * ================================================================================================
 * Class Vertex
 * ================================================================================================
 */
template <class T>
class Vertex {
	T info;
	vector<Edge<T>*> outgoing;  // adj
	vector<Edge<T>*> incoming;
	Edge<T>* addEdge(Vertex<T> *dest, double c, double f);
	Vertex(T in);

	double best = 0;
	bool visited = false;
	Vertex<T>* path = nullptr;

public:
	T getInfo() const;
	Edge<T>* edgeTo(Vertex<T>* dest) const;
	Edge<T>* edgeFrom(Vertex<T>* dest) const;
	vector<Edge<T>*> getAdj() const;
	friend class Graph<T>;
};

template <class T>
Edge<T>* Vertex<T>::edgeTo(Vertex<T>* dest) const {
	for (auto edge : outgoing)
		if (edge->dest == dest) return edge;

	return nullptr;
}


template <class T>
Edge<T>* Vertex<T>::edgeFrom(Vertex<T>* dest) const {
	for (auto edge : incoming)
		if (edge->orig == dest) return edge;

	return nullptr;
}

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *dest, double c, double f) {
	Edge<T> * e = new Edge<T>(this, dest, c, f);
	this->outgoing.push_back(e);
	dest->incoming.push_back(e);
	return e;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
vector<Edge<T> *> Vertex<T>::getAdj() const {
	return this->outgoing;
}


/* ================================================================================================
 * Class Edge
 * ================================================================================================
 */
template <class T>
class Edge {
	Vertex<T>* orig;
	Vertex<T>* dest;
	double capacity;
	double flow;
	Edge(Vertex<T>* o, Vertex<T>* d, double c, double f = 0);

public:
	double getFlow() const;
	Vertex<T>* getDest() const;

	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T>* o, Vertex<T>* d, double w, double f):
	orig(o), dest(d), capacity(w), flow(f) {}

template <class T>
double Edge<T>::getFlow() const {
	return flow;
}

template <class T>
Vertex<T>* Edge<T>::getDest() const {
	return dest;
}


/* ================================================================================================
 * Class Graph
 * ================================================================================================
 */
template <class T>
class Graph {
	vector<Vertex<T>*> vertexSet;
	Vertex<T>* findVertex(const T &inf) const;
public:
	vector<Vertex<T>*> getVertexSet() const;
	Vertex<T>* addVertex(const T &in);
	Edge<T>* addEdge(const T &sourc, const T &dest, double c, double f = 0);
	vector<Vertex<T>*> getPath(Vertex<T>* origin, Vertex<T>* dest) const;
	void fordFulkerson(T source, T target);

};

template <class T>
vector<Vertex<T>*> Graph<T>::getPath(Vertex<T>* origin, Vertex<T>* dest) const {
	vector<Vertex<T>*> path;
	Vertex<T>* curr = dest;

	do {
		path.push_back(curr);
		curr = curr->path;
	} while (curr != nullptr && curr != origin);

	if (curr == origin) path.push_back(origin);

	reverse(path.begin(), path.end());

	return path;
}

template <class T>
Vertex<T>* Graph<T>::addVertex(const T &in) {
	Vertex<T>* v = findVertex(in);
	if (v != nullptr)
		return v;
	v = new Vertex<T>(in);
	vertexSet.push_back(v);
	return v;
}

template <class T>
Edge<T>* Graph<T>::addEdge(const T &sourc, const T &dest, double c, double f) {
	auto s = findVertex(sourc);
	auto d = findVertex(dest);
	if (s == nullptr || d == nullptr)
		return nullptr;
	else
		return s->addEdge(d, c, f);
}

template <class T>
Vertex<T>* Graph<T>::findVertex(const T & inf) const {
	for (auto v : vertexSet)
		if (v->info == inf)
			return v;
	return nullptr;
}

template <class T>
vector<Vertex<T>*> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/**
 * Finds the maximum flow in a graph using the Ford Fulkerson algorithm
 * (with the improvement of Edmonds-Karp).
 * Assumes that the graph forms a flow network from source vertex 's'
 * to sink vertex 't' (distinct vertices).
 * Receives as arguments the source and target vertices (identified by their contents).
 * The result is defined by the "flow" field of each edge.
 */
template <class T>
void Graph<T>::fordFulkerson(T src, T tar) {
	Vertex<T>* source = findVertex(src);
    Vertex<T>* target = findVertex(tar);
    double max_flow = 0;

	const auto bfs = [&]()
			-> vector<Vertex<T>*> {

		// reset bfs
		for (auto v : vertexSet) {
			v->visited = false;
			v->path = nullptr;
			v->best = INF;
		}

		// vertices reachable by incomplete augmenting paths
		queue<Vertex<T>*> q;
		q.push(source);

		while (!q.empty()) {
			Vertex<T>* cur = q.front();
			q.pop();

			// edges out of the current vertex, which shall
			// carry extra flow in their direction
			for (auto next : cur->outgoing) {
				double residue = next->capacity - next->flow;
				if (!next->dest->visited && residue > 0) {
					next->dest->visited = true;
					next->dest->path = cur;
					q.push(next->dest);
				}
			}

			// edges into the current vertex, which shall
			// lose flow in their direction
			for (auto next : cur->incoming) {
				double residue = next->flow;
				if (!next->orig->visited && residue > 0) {
					next->orig->visited = true;
					next->orig->path = cur;
					q.push(next->orig);
				}
			}
		}

		// if there is an augmenting path, return it,
		// otherwise return an empty path.
		if (target->visited) {
			return getPath(source, target);
		} else {
			return {};
		}
	};

	for (auto v : vertexSet) {
    	for (auto e : v->outgoing)
    		e->flow = 0; // reset all flow
	}

    vector<Vertex<T>*> Path = bfs();
    while (Path.size() > 0) {
    	// Find max flow that can be sent down the path
    	double df = INF;
    	for (auto it = Path.begin() + 1; it != Path.end(); ++it) {
    		Edge<T>* to = (*(it - 1))->edgeTo(*it);
    		Edge<T>* from = (*(it - 1))->edgeFrom(*it);
    		if (to)
    			df = min(df, to->capacity - to->flow);
    		if (from)
    			df = min(df, from->flow);
    	}
    	for (auto it = Path.begin() + 1; it != Path.end(); ++it) {
    		Edge<T>* to = (*(it - 1))->edgeTo(*it);
    		Edge<T>* from = (*(it - 1))->edgeFrom(*it);
    		if (to)
    			to->flow += df;
    		if (from)
    			from->flow -= df;
    	}
    	max_flow += df;
    	Path = bfs();
    }

    cout << "Max flow: " << max_flow << endl;
}


#endif /* GRAPH_H_ */
