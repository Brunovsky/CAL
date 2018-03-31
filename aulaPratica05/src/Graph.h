/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>
#include "MutablePriorityQueue.h"

using namespace std;

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
	T info;                // contents
	vector<Edge<T>> adj;   // outgoing edges
	bool visited = false;          // auxiliary field
	double dist = 0;
	Vertex<T> *path = nullptr;
	int queueIndex = 0; 		// required by MutablePriorityQueue

	bool processing = false;
	void addEdge(Vertex<T> *dest, double w);

public:
	void resetPath(); // Resets auxiliary fields
	Vertex(T in);
	bool operator<(Vertex<T> & vertex) const; // required by MutablePriorityQueue
	T getInfo() const;
	double getDist() const;
	Vertex *getPath() const;
	friend class Graph<T>;
	friend class MutablePriorityQueue<Vertex<T>>;
};

template <class T>
inline void Vertex<T>::resetPath() {
	visited = false;
	dist = 0;
	path = nullptr;
	queueIndex = 0;
	processing = false;
}


template <class T>
Vertex<T>::Vertex(T in): info(in), visited(false) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
	adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
	return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
	return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
	return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
	return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
	Vertex<T> * dest;      // destination vertex
	double weight;         // edge weight
public:
	Edge(Vertex<T> *d, double w);
	friend class Graph<T>;
	friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
	vector<Vertex<T> *> vertexSet;    // vertex set
	vector<vector<pair<Vertex<T>*, double>>> M;  // adjacency matrix

public:
	void resetPath(); // Resets all vertices' auxiliary fields
	Vertex<T> *findVertex(const T &in) const;
	bool addVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int getNumVertex() const;
	vector<Vertex<T> *> getVertexSet() const;

	// Fp05 - single source
	void dijkstraShortestPath(const T &s);
	void dijkstraShortestPathOld(const T &s);
	void unweightedShortestPath(const T &s);
	void bellmanFordShortestPath(const T &s);
	vector<T> getPath(const T &origin, const T &dest) const;

	// Fp05 - all pairs
	void floydWarshallShortestPath();
	vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;

};

template<class T>
void Graph<T>::resetPath() {
	M.clear();
	for (auto v : vertexSet) v->resetPath();
}

template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
}

template <class T>
vector<Vertex<T> *> Graph<T>::getVertexSet() const {
	return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
	for (auto v : vertexSet)
		if (v->info == in)
			return v;
	return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
	if (findVertex(in) != NULL)
		return false;
	vertexSet.push_back(new Vertex<T>(in));
	return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
	auto v1 = findVertex(sourc);
	auto v2 = findVertex(dest);
	if (v1 == nullptr || v2 == nullptr)
		return false;
	v1->addEdge(v2,w);
	return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
	resetPath();
	MutablePriorityQueue<Vertex<T>> quw;
	auto first = findVertex(origin);
	if (first == nullptr) return;
	quw.insert(first);

	while (!quw.empty()) {
		auto source = quw.extractMin();
		source->visited = true;

		for (auto edge : source->adj) {
			auto target = edge.dest;
			if (target->visited) continue;

			double candidate_dist = source->dist + edge.weight;

			if (target->path == NULL) {
				target->path = source;
				target->dist = candidate_dist;
				quw.insert(target);
			} else if (target->dist > candidate_dist) {
				target->path = source;
				target->dist = candidate_dist;
				quw.decreaseKey(target);
			}
		}
	}

	return;
}

template<class T>
vector<T> Graph<T>::getPath(const T &origin, const T &dest) const {
	vector<T> res;
	auto first = findVertex(origin);
	auto last = findVertex(dest);
	if (first == nullptr || last == nullptr) return res;

	res.push_back(last->info);
	auto path = last;

	do {
		path = path->path;
		res.push_back(path->info);
	} while(path != first);

	reverse(res.begin(), res.end());
	return res;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &origin) {
	resetPath();
	queue<Vertex<T>*> unprocessed;
	auto first = findVertex(origin);
	if (first == nullptr) return;
	unprocessed.push(first);
	first->visited = true;

	while (!unprocessed.empty()) {
		auto source = unprocessed.front();
		unprocessed.pop();
		for (auto edge : source->adj) {
			auto target = edge.dest;
			if (target->visited) continue;
			else {
				target->dist = source->dist + 1;
				target->path = source;
				target->visited = true;
				unprocessed.push(target);
			}
		}
	}

	return;
}

template<class T>
void Graph<T>::bellmanFordShortestPath(const T &origin) {
	resetPath();
	auto first = findVertex(origin);
	if (first == nullptr) return;
	first->visited = true;
	int vertices = getNumVertex();

	for (int i = 1; i < vertices; ++i) {
		for (auto source : vertexSet) {
			for (auto edge : source->adj) {
				auto target = edge.dest;
				double candidate = source->dist + edge.weight;
				if (target->dist > candidate) {
					target->dist = candidate;
					target->path = source;
				}
				if (!target->visited) {
					target->dist = candidate;
					target->path = source;
					target->visited = true;
				}
			}
		}
	}

	for (auto source : vertexSet) {
		for (auto edge : source->adj) {
			auto target = edge.dest;
			if (target->dist > source->dist + edge.weight)
				throw "Negative weight cycle found";
		}
	}

	return;
}


/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
	resetPath();
	// adjMatrix[i][j] -> <predecessor of j in best path from i to j, dist>
	int vertices = getNumVertex();
	M.resize(vertices);

	for (int i = 0; i < vertices; ++i) {
		vertexSet[i]->queueIndex = i;
		M[i].resize(vertices);
	}

	for (auto v : vertexSet) {
		int i = v->queueIndex;
		for (int k = 0; k < vertices; ++k)
			M[i][k] = {nullptr, 0};
		for (auto edge : v->adj) {
			int j = edge.dest->queueIndex;
			M[i][j] = {v, edge.weight};
		}
	}

	for (auto intermediate : vertexSet) {
		int k = intermediate->queueIndex;
		for (auto source : vertexSet) {
			int i = source->queueIndex;
			for (auto target : vertexSet) {
				int j = target->queueIndex;
				// Check both paths i->k and k->j exist already.
				if (M[i][k].first != nullptr && M[k][j].first != nullptr) {
					double dist = M[i][k].second + M[k][j].second;
					if (dist < M[i][j].second || M[i][j].first == nullptr) {
						M[i][j].first = M[k][j].first;
						M[i][j].second = dist;
					}
				}
			}
		}
	}

	for (auto v : vertexSet) {
		int i = v->queueIndex;
		if (M[i][i].second < 0) // Check main diagonal for negative cycles.
			throw "Negative cycle found";
	}

	return;
}

template<class T>
vector<T> Graph<T>::getfloydWarshallPath(const T &origin, const T &dest) const {
	vector<T> res;
	auto source = findVertex(origin);
	auto target = findVertex(dest);
	if (source == nullptr || target == nullptr) return res;

	int i = source->queueIndex;
	auto path = target;
	res.push_back(target->info);

	do {
		int k = path->queueIndex;
		path = M[i][k].first;
		res.push_back(path->info);
	} while (path != source);

	reverse(res.begin(), res.end());
	return res;
}


#endif /* GRAPH_H_ */
