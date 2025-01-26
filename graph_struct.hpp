#include <stdexcept>
#include <vector> // - временная мера для тестирования


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

// База вершины
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
    ConcreteVertex(int id, std::string label) : id_(id), label_(label) {}

    int id() const override {
        return id_;
    }

    std::string label() const override {
        return label_;
    }
};

// База ребра с весом W
template <typename W>
class BaseEdge {
public:
    virtual BaseVertex* source() const = 0;
    virtual BaseVertex* target() const = 0;
    virtual W weight() const = 0;
    virtual ~BaseEdge() = default;
};

// Конкретная реализация ребра
template <typename W>
class ConcreteEdge : public BaseEdge<W> {
private:
    BaseVertex* source_;
    BaseVertex* target_;
    W weight_;

public:
    ConcreteEdge(BaseVertex* source, BaseVertex* target, W weight)
            : source_(source), target_(target), weight_(weight) {}

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

// Абстрактный базовый класс для графа
template <typename V, typename E, typename W>
class Graph {
public:
    virtual void addVertex(const V& vertex) = 0;
    virtual void addEdge(const E& edge) = 0;
    virtual const std::vector<V>& vertices() const = 0;
    virtual const std::vector<E>& edges() const = 0;
    virtual ~Graph() = default;
};

// Ориентированный граф
template <typename V, typename E, typename W>
class DirectedGraph : public Graph<V, E, W> {
private:
    std::vector<V> vertices_;
    std::vector<E> edges_;

public:
    void addVertex(const V& vertex) override {
        vertices_.push_back(vertex);
    }

    void addEdge(const E& edge) override {
        edges_.push_back(edge);
    }

    const std::vector<V>& vertices() const override {
        return vertices_;
    }

    const std::vector<E>& edges() const override {
        return edges_;
    }
};

// Неориентированный граф
template <typename V, typename E, typename W>
class UndirectedGraph : public Graph<V, E, W> {
private:
    std::vector<V> vertices_;
    std::vector<E> edges_;

public:
    void addVertex(const V& vertex) override {
        vertices_.push_back(vertex);
    }

    void addEdge(const E& edge) override {
        edges_.push_back(edge);
        edges_.push_back(E(edge.target(), edge.source(), edge.weight()));
    }

    const std::vector<V>& vertices() const override {
        return vertices_;
    }

    const std::vector<E>& edges() const override {
        return edges_;
    }
};




