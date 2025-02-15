#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <ostream>
#include <iomanip>
#include <concepts>
#include <chrono>
#include <ctime>
#include <functional>


// Перегрузка оператора << для std::pair
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& p) {
    os << p.first << ":" << p.second;
    return os;
}

// Перегрузка оператора << для std::tuple
template<typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
    std::apply([&os](const auto&... args) {
        size_t index = 0;
        ((os << (index++ > 0 ? ":" : "") << args), ...);
    }, t);
    return os;
}

// Специализация std::hash для std::pair
namespace std {
    template<typename T1, typename T2>
    struct hash<std::pair<T1, T2>> {
        size_t operator()(const std::pair<T1, T2>& p) const {
            auto h1 = std::hash<T1>{}(p.first);
            auto h2 = std::hash<T2>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };

    template<typename T1, typename T2, typename T3>
    struct hash<std::tuple<T1, T2, T3>> {
        size_t operator()(const std::tuple<T1, T2, T3>& t) const {
            auto h1 = std::hash<T1>{}(std::get<0>(t));
            auto h2 = std::hash<T2>{}(std::get<1>(t));
            auto h3 = std::hash<T3>{}(std::get<2>(t));
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}

// Concept для проверки возможности хеширования
template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

// Структура для представления блока
template<Hashable T>
struct Block {
    size_t index;          // Индекс блока
    T data;                // Данные блока
    std::string prevHash;  // Хеш предыдущего блока
    std::string hash;      // Хеш текущего блока
    std::time_t timestamp; // Временная метка

    // Конструктор блока
    Block(size_t idx, const T& data, const std::string& prevHash)
            : index(idx), data(data), prevHash(prevHash), timestamp(std::time(nullptr))
    {
        hash = calculateHash();
    }

    // Проверка, совпадает ли текущий хеш с вычисленным
    bool isHashValid() const {
        return hash == calculateHash();
    }

private:
    // Метод для вычисления хеша блока
    std::string calculateHash() const {
        std::ostringstream oss;
        oss << index << data << prevHash << timestamp; // Для вычисления хеша включаем поля
        std::string toHash = oss.str();

        std::hash<std::string> hasher;
        size_t hashedValue = hasher(toHash);

        std::ostringstream hashStream;
        hashStream << std::hex << std::setw(16) << std::setfill('0') << hashedValue;
        return hashStream.str();
    }
};

// Класс Blockchain
template<Hashable T>
class Blockchain {
private:
    std::vector<Block<T>> chain; // Цепочка блоков

public:
    Blockchain() {
        Block<T> genesisBlock(0, T{}, "0");
        chain.push_back(genesisBlock);
    }

    // Добавление нового блока с данными data.
    void addBlock(const T& data) {
        size_t newIndex = chain.size();
        std::string prevHash = chain.back().hash;
        Block<T> newBlock(newIndex, data, prevHash);
        chain.push_back(newBlock);
    }

    // Получение цепочки блоков.
    const std::vector<Block<T>>& getChain() const {
        return chain;
    }

    // Проверка целостности цепочки блоков.
    bool isValid() const {
        for (size_t i = 1; i < chain.size(); ++i) {
            const Block<T>& currentBlock = chain[i];
            const Block<T>& previousBlock = chain[i - 1];
            if (!currentBlock.isHashValid() ||
                currentBlock.prevHash != previousBlock.hash)
                return false;
        }
        return true;
    }
};