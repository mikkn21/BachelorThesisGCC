#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <set>
#include <map>
#include <iostream>

template <typename T>
class Vertex {
public:
    T value;
    std::set<Vertex<T>*> neighbors;

    Vertex(T value) : value(value) {}
    
};



template <typename T>
class Graph {
    std::list<Vertex<T>> vertices;
    
public:
    void add_vertex(T vertex) {
        if (!has_vertex(vertex)){
            vertices.push_back(Vertex<T>(vertex));
        }
    }

    void remove_vertex(T vertex) {
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            if (it->value == vertex) {
                vertices.erase(it);
                break;
            }
        }
    }

    bool has_vertex(T vertex) {
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            if (it->value == vertex) {
                return true;
            }
        }
        return false;
    }

    auto get_vertex(T value) {
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            if (it->value == value) {
                return it;
            }
        }
        throw std::runtime_error("Vertex not found");
    }

    void add_edge(T v, T u) {
        auto v_vertex = get_vertex(v);
        auto u_vertex = get_vertex(u);
        
        v_vertex->neighbors.insert(&(*u_vertex));
        u_vertex->neighbors.insert(&(*v_vertex));
    }

    void remove_edge(T v, T u) {
        auto v_vertex = get_vertex(v);
        auto u_vertex = get_vertex(u);
        
        v_vertex->neighbors.erase(&(*u_vertex));
        u_vertex->neighbors.erase(&(*v_vertex));  
    }

    size_t degree(T vertex) {
        return get_vertex(vertex)->neighbors.size();
    }

    const std::set<Vertex<T>*> &get_neighbors(T v) {
        // std::cout << "get_neighbors length: " << get_vertex(v).neighbors.size() << std::endl;
        return get_vertex(v)->neighbors;
    }   

    const std::list<Vertex<T>> &get_vertices() {
        return vertices;
    }
};


 #endif  // GRAPH_HPP
