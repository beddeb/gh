#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include "block_chain.hpp"

// Концепт для вершины графа
template <typename T>
concept Vertex = requires(T v) {
    { v.id() } -> std::convertible_to<int>;
    { v.label() } -> std::convertible_to<std::string>;
};

// Концепт для ребра графа с весом W
template <typename E, typename W>
concept Edge = requires(E e) {
    { e.source() } -> Vertex;
    { e.target() } -> Vertex;
    { e.weight() } -> std::same_as<W>;
};

// Базовый класс для вершины
class BaseVertex {
public:
    virtual int id() const = 0;
    virtual std::string label() const = 0;
    virtual ~BaseVertex() = default;
};

// Конкретная реализация вершины
class ConcreteVertex : public BaseVertex {
private:
    int id_;
    std::string label_;
public:
    // Конструктор по умолчанию (необходим для использования T{} в Blockchain)
    ConcreteVertex() : id_(0), label_("") {}

    ConcreteVertex(int id, std::string label) : id_(id), label_(label) {}

    int id() const override {
        return id_;
    }

    std::string label() const override {
        return label_;
    }
};

// Определяем оператор вывода для ConcreteVertex в глобальном пространстве имён.
inline std::ostream& operator<<(std::ostream& os, const ConcreteVertex& v) {
    os << "(" << v.id() << ", " << v.label() << ")";
    return os;
}

// Базовый класс для ребра с весом W
template <typename W>
class BaseEdge {
public:
    virtual BaseVertex* source() const = 0;
    virtual BaseVertex* target() const = 0;
    virtual W weight() const = 0;
    virtual ~BaseEdge() = default;
};

// Конкретная реализация ребра.
// Добавим конструктор по умолчанию, что бы T{} сработал в Blockchain.
template <typename W>
class ConcreteEdge : public BaseEdge<W> {
private:
    BaseVertex* source_;
    BaseVertex* target_;
    W weight_;
public:
    // Конструктор по умолчанию
    ConcreteEdge() : source_(nullptr), target_(nullptr), weight_(W{}) { }

    ConcreteEdge(BaseVertex* source, BaseVertex* target, W weight)
            : source_(source), target_(target), weight_(weight) { }

    BaseVertex* source() const override {
        return source_;
    }

    BaseVertex* target() const override {
        return target_;
    }

    W weight() const override {
        return weight_;
    }
};

namespace std {
    template <>
    struct hash<ConcreteVertex> {
        size_t operator()(const ConcreteVertex &v) const {
            size_t h1 = std::hash<int>{}(v.id());
            size_t h2 = std::hash<std::string>{}(v.label());
            return h1 ^ (h2 << 1);
        }
    };

    template <>
    struct hash<ConcreteEdge<int>> {
    size_t operator()(const ConcreteEdge<int>& e) const {
        size_t h1 = std::hash<int>{}(e.source() ? e.source()->id() : 0);
        size_t h2 = std::hash<int>{}(e.target() ? e.target()->id() : 0);
        size_t h3 = std::hash<int>{}(e.weight());
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};
}

template <typename W>
inline std::ostream& operator<<(std::ostream& os, const ConcreteEdge<W>& e) {
    os << "("
       << (e.source() ? e.source()->id() : -1) << "->"
       << (e.target() ? e.target()->id() : -1) << ", "
       << e.weight() << ")";
    return os;
}

// Абстрактный базовый класс для графа
template <typename V, typename E, typename W>
class Graph {
public:
    virtual void addVertex(const V& vertex) = 0;
    virtual void addEdge(const E& edge) = 0;
    virtual std::vector<V> vertices() const = 0;
    virtual std::vector<E> edges() const = 0;
    virtual ~Graph() = default;
};

// Ориентированный граф с хранением вершин и ребер в Blockchain.
template <typename V, typename E, typename W>
class DirectedGraph : public Graph<V, E, W> {
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

    // Получаем вершины (пропуская генезис-блок)
    std::vector<V> vertices() const override {
        std::vector<V> vec;
        const auto& chain = verticesChain.getChain();
        for (size_t i = 1; i < chain.size(); ++i)
            vec.push_back(chain[i].data);
        return vec;
    }

    // Получаем ребра (также пропуская генезис-блок)
    std::vector<E> edges() const override {
        std::vector<E> vec;
        const auto& chain = edgesChain.getChain();
        for (size_t i = 1; i < chain.size(); ++i)
            vec.push_back(chain[i].data);
        return vec;
    }
};

// Неориентированный граф (при добавлении ребра сохраняется его зеркало).
template <typename V, typename E, typename W>
class UndirectedGraph : public Graph<V, E, W> {
private:
    Blockchain<V> verticesChain;
    Blockchain<E> edgesChain;
public:
    void addVertex(const V& vertex) override {
        verticesChain.addBlock(vertex);
    }

    void addEdge(const E& edge) override {
        edgesChain.addBlock(edge);
        // Предполагается, что тип E имеет конструктор: E(target, source, weight)
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