#pragma once

#include <stdexcept>
#include "block_chain.hpp"


// Концепт для вершины графа
template <typename T, typename VW, typename VL>
concept Vertex = requires(T v) {
    { v.id() } -> std::convertible_to<int>;
    { v.label() } -> std::convertible_to<VL>;
    { v.weight() } -> std::same_as<VW>;
};

// Концепт для ребра графа с весом W
template <typename E, typename W, typename V>
concept Edge = requires(E e, V source_v, V target_v) {
    { e.source() } -> std::same_as<V>;
    { e.target() } -> std::same_as<V>;
    { e.weight() } -> std::same_as<W>;
};

// Базовый класс для вершины
template <typename VW, typename VL> // VW - тип веса вершины, VL - тип метки вершины
class BaseVertex {
public:
    virtual int id() const = 0;
    virtual VL label() const = 0;
    virtual VW weight() const = 0;
    virtual ~BaseVertex() = default;
};

// Конкретная реализация вершины
template <typename VW, typename VL> // VW - тип веса вершины, VL - тип метки вершины
class ConcreteVertex : public BaseVertex<VW, VL> {
private:
    int id_;
    VL label_;
    VW weight_;
public:
    ConcreteVertex() : id_(0), label_(VL{}), weight_(VW{}) {}

    ConcreteVertex(int id, VL label, VW weight)
            : id_(id), label_(label), weight_(weight) {}

    int id() const override {
        return id_;
    }

    VL label() const override {
        return label_;
    }

    VW weight() const override {
        return weight_;
    }
};

// Оператор вывода для ConcreteVertex
template <typename VW, typename VL>
inline std::ostream& operator<<(std::ostream& os, const ConcreteVertex<VW, VL>& v) {
    os << "(" << v.id() << ", " << v.label() << ", " << v.weight() << ")";
    return os;
}

// Базовый класс для ребра с весом W
template <typename W, typename V>
class BaseEdge {
public:
    virtual V source() const = 0;
    virtual V target() const = 0;
    virtual W weight() const = 0;
    virtual ~BaseEdge() = default;
};

// Конкретная реализация ребра
template <typename W, typename V>
class ConcreteEdge : public BaseEdge<W, V> {
private:
    V source_;
    V target_;
    W weight_;
public:
    ConcreteEdge() : source_(), target_(), weight_(W{}) { }

    ConcreteEdge(V source, V target, W weight)
            : source_(source), target_(target), weight_(weight) { }

    V source() const override {
        return source_;
    }

    V target() const override {
        return target_;
    }

    W weight() const override {
        return weight_;
    }
};

// Обновляем хэш-функцию для ConcreteVertex
namespace std {
    template <typename VW, typename VL>
    struct hash<ConcreteVertex<VW, VL>> {
    size_t operator()(const ConcreteVertex<VW, VL>& v) const {
        size_t h1 = std::hash<int>{}(v.id());
        size_t h2 = std::hash<VL>{}(v.label());
        size_t h3 = std::hash<VW>{}(v.weight());
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

template <typename W, typename VW, typename VL>
struct hash<ConcreteEdge<W, ConcreteVertex<VW, VL>>> {
    size_t operator()(const ConcreteEdge<W, ConcreteVertex<VW, VL>>& e) const {
        size_t h1 = std::hash<int>{}(e.source().id());
        size_t h2 = std::hash<int>{}(e.target().id());
        size_t h3 = std::hash<W>{}(e.weight());
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
}

template <typename W, typename V>
inline std::ostream& operator<<(std::ostream& os, const ConcreteEdge<W, V>& e) {
    os << "("
       << e.source().id() << "->"
       << e.target().id() << ", "
       << e.weight() << ")";
    return os;
}

// Абстрактный базовый класс для графа (добавляем VW и VL)
template <typename V, typename E, typename W, typename VW, typename VL>
class Graph {
public:
    virtual void addVertex(const V& vertex) = 0;
    virtual void addEdge(const E& edge) = 0;
    virtual std::vector<V> vertices() const = 0;
    virtual std::vector<E> edges() const = 0;
    virtual ~Graph() = default;
};

// Ориентированный граф
template <typename V, typename E, typename W, typename VW, typename VL>
class DirectedGraph : public Graph<V, E, W, VW, VL> {
private:
    Blockchain<V> verticesChain;
    Blockchain<E> edgesChain;
public:
    void addVertex(const V& vertex) override {
        verticesChain.addBlock(vertex);
    }

    void addEdge(const E& edge) override {
        edgesChain.addBlock(edge);
    }

    std::vector<V> vertices() const override {
        std::vector<V> vec;
        const auto& chain = verticesChain.getChain();
        for (size_t i = 1; i < chain.size(); ++i)
            vec.push_back(chain[i].data);
        return vec;
    }

    std::vector<E> edges() const override {
        std::vector<E> vec;
        const auto& chain = edgesChain.getChain();
        for (size_t i = 1; i < chain.size(); ++i)
            vec.push_back(chain[i].data);
        return vec;
    }
};

// Неориентированный граф
template <typename V, typename E, typename W, typename VW, typename VL>
class UndirectedGraph : public Graph<V, E, W, VW, VL> {
private:
    Blockchain<V> verticesChain;
    Blockchain<E> edgesChain;
public:
    void addVertex(const V& vertex) override {
        verticesChain.addBlock(vertex);
    }

    void addEdge(const E& edge) override {
        edgesChain.addBlock(edge);
        edgesChain.addBlock(E(edge.target(), edge.source(), edge.weight()));
    }

    std::vector<V> vertices() const override {
        std::vector<V> vec;
        const auto& chain = verticesChain.getChain();
        for (size_t i = 1; i < chain.size(); ++i)
            vec.push_back(chain[i].data);
        return vec;
    }

    std::vector<E> edges() const override {
        std::vector<E> vec;
        const auto& chain = edgesChain.getChain();
        for (size_t i = 1; i < chain.size(); ++i)
            vec.push_back(chain[i].data);
        return vec;
    }
};