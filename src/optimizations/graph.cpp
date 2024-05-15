#include "graph.hpp"

template<typename T>
Graph<T>::Graph() {}

template<typename T>
void Graph<T>::add_vertex(const T& vertex) {
    vertices.insert(vertex);
}

template<typename T>
void Graph<T>::remove_vertex(const T& vertex) {
    vertices.erase(vertex);
    edges.erase(vertex);
    for (auto it = edges.begin(); it != edges.end(); ++it) {
        remove_edge(it->first, vertex);
    }
}

template<typename T>
void Graph<T>::add_edge(const T& origin, const T& destination) {
    edges[origin].insert(destination);
}

template<typename T>
void Graph<T>::remove_edge(const T& origin, const T& destination) {
    edges[origin].erase(destination);
}