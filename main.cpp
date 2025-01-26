#include "graph_struct.hpp"
#include <iostream>


int main() {
    // Создаем вершины
    ConcreteVertex v1(1, "A");
    ConcreteVertex v2(2, "B");

    // Создаем ребра
    ConcreteEdge<int> e1(&v1, &v2, 10);

    // Создаем ориентированный граф
    DirectedGraph<ConcreteVertex, ConcreteEdge<int>, int> directedGraph;
    directedGraph.addVertex(v1);
    directedGraph.addVertex(v2);
    directedGraph.addEdge(e1);

    // Создаем неориентированный граф
    UndirectedGraph<ConcreteVertex, ConcreteEdge<int>, int> undirectedGraph;
    undirectedGraph.addVertex(v1);
    undirectedGraph.addVertex(v2);
    undirectedGraph.addEdge(e1);

    // Выводим информацию о графах
    std::cout << "Directed Graph Vertices:\n";
    for (const auto& vertex : directedGraph.vertices()) {
        std::cout << "ID: " << vertex.id() << ", Label: " << vertex.label() << "\n";
    }

    std::cout << "Directed Graph Edges:\n";
    for (const auto& edge : directedGraph.edges()) {
        std::cout << "Source: " << edge.source()->label() << ", Target: " << edge.target()->label() << ", Weight: " << edge.weight() << "\n";
    }

    std::cout << "Undirected Graph Vertices:\n";
    for (const auto& vertex : undirectedGraph.vertices()) {
        std::cout << "ID: " << vertex.id() << ", Label: " << vertex.label() << "\n";
    }

    std::cout << "Undirected Graph Edges:\n";
    for (const auto& edge : undirectedGraph.edges()) {
        std::cout << "Source: " << edge.source()->label() << ", Target: " << edge.target()->label() << ", Weight: " << edge.weight() << "\n";
    }

    return 0;
}