#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_set>
#include <unordered_map>

template<typename T>
class Graph {
    std::unordered_set<T> vertices;
    std::unordered_map<T, std::unordered_set<T>> edges;

public:
    Graph();
    void add_vertex(const T& vertex);
    void remove_vertex(const T& vertex);
    void add_edge(const T& origin, const T& destination);
    void remove_edge(const T& origin, const T& destination);
};


#endif  // GRAPH_HPP