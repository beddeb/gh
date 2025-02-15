#include <iostream>
#include "graph_struct.hpp"


int main() {
    ConcreteVertex v1(1, "VertexA");
    ConcreteVertex v2(2, "VertexB");
    ConcreteVertex v3(3, "VertexC");

    ConcreteEdge<int> e1(&v1, &v2, 10);
    ConcreteEdge<int> e11(&v1, &v2, 20);
    ConcreteEdge<int> e2(&v2, &v3, 20);

    DirectedGraph<ConcreteVertex, ConcreteEdge<int>, int> directedGraph;
    directedGraph.addVertex(v1);
    directedGraph.addVertex(v2);
    directedGraph.addVertex(v3);
    directedGraph.addEdge(e1);
    directedGraph.addEdge(e11);
    directedGraph.addEdge(e2);

    std::cout << "Directed graph:" << std::endl;
    std::cout << "Vertices:" << std::endl;
    for (const auto& vertex : directedGraph.vertices()) {
        std::cout << vertex.id() << " - " << vertex.label() << std::endl;
    }
    std::cout << "Edges:" << std::endl;
    for (const auto& edge : directedGraph.edges()) {
        std::cout << edge.source()->id() << " -> "
                  << edge.target()->id() << " (Weight: " << edge.weight() << ")" << std::endl;
    }

    UndirectedGraph<ConcreteVertex, ConcreteEdge<int>, int> undirectedGraph;
    undirectedGraph.addVertex(v1);
    undirectedGraph.addVertex(v2);
    undirectedGraph.addVertex(v3);
    undirectedGraph.addEdge(e1);
    undirectedGraph.addEdge(e2);

    std::cout << "\nUndirected graph:" << std::endl;
    std::cout << "Vertices:" << std::endl;
    for (const auto& vertex : undirectedGraph.vertices()) {
        std::cout << vertex.id() << " - " << vertex.label() << std::endl;
    }
    std::cout << "Edges:" << std::endl;
    for (const auto& edge : undirectedGraph.edges()) {
        std::cout << edge.source()->id() << " -- "
                  << edge.target()->id() << " (Weight: " << edge.weight() << ")" << std::endl;
    }

    return 0;
}