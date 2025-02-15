#include <iostream>
#include "graph_struct.hpp"

int main() {
    // Пример с ориентированным графом
    std::cout << "\n=== Directed Graph ===\n";
    using CityVertex = ConcreteVertex<int>;
    using RoadEdge = ConcreteEdge<double, CityVertex>;
    DirectedGraph<CityVertex, RoadEdge, double, int> cityGraph;

    CityVertex moscow(1, "Moscow", 12000000);
    CityVertex spb(2, "Saint Petersburg", 5300000);
    CityVertex novosibirsk(3, "Novosibirsk", 1600000);
    CityVertex ekaterinburg(4, "Ekaterinburg", 1500000);

    cityGraph.addVertex(moscow);
    cityGraph.addVertex(spb);
    cityGraph.addVertex(novosibirsk);
    cityGraph.addVertex(ekaterinburg);

    RoadEdge moscow_spb(moscow, spb, 700.0);
    RoadEdge moscow_novosibirsk(moscow, novosibirsk, 3350.0);
    RoadEdge spb_ekaterinburg(spb, ekaterinburg, 2300.0);
    RoadEdge novosibirsk_ekaterinburg(novosibirsk, ekaterinburg, 1400.0);

    cityGraph.addEdge(moscow_spb);
    cityGraph.addEdge(moscow_novosibirsk);
    cityGraph.addEdge(spb_ekaterinburg);
    cityGraph.addEdge(novosibirsk_ekaterinburg);

    // Выводим информацию о графе
    std::cout << "Cities (vertices):\n";
    for (const auto& city : cityGraph.vertices()) {
        std::cout << "City: " << city << " (population: " << city.weight() << ")\n";
    }

    std::cout << "\nRoads (edges):\n";
    for (const auto& road : cityGraph.edges()) {
        std::cout << "Road: " << road << " (distance: " << road.weight() << " km)\n";
    }

    // Пример с неориентированным графом
    std::cout << "\n=== Undirected Graph ===\n";
    UndirectedGraph<CityVertex, RoadEdge, double, int> undirectedCityGraph;

    undirectedCityGraph.addVertex(moscow);
    undirectedCityGraph.addVertex(spb);
    undirectedCityGraph.addVertex(novosibirsk);
    undirectedCityGraph.addVertex(ekaterinburg);

    undirectedCityGraph.addEdge(moscow_spb);
    undirectedCityGraph.addEdge(moscow_novosibirsk);

    std::cout << "\nCities in undirected graph:\n";
    for (const auto& city : undirectedCityGraph.vertices()) {
        std::cout << "City: " << city << " (population: " << city.weight() << ")\n";
    }

    std::cout << "\nRoads in undirected graph:\n";
    for (const auto& road : undirectedCityGraph.edges()) {
        std::cout << "Road: " << road << " (distance: " << road.weight() << " km)\n";
    }

    return 0;
}