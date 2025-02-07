#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <concepts>
#include <chrono>


// Concept для проверки, что тип можно хешировать
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
            : index(idx), data(data), prevHash(prevHash), timestamp(std::time(nullptr)) {
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
        oss << index << data << prevHash << timestamp; // Включаем timestamp в хеш
        std::string toHash = oss.str();

        std::hash<std::string> hasher;
        size_t hashedValue = hasher(toHash);

        // Преобразуем хеш в строку
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
    // Конструктор для инициализации блокчейна
    Blockchain() {
        // Создаем генезис-блок (первый блок)
        Block<T> genesisBlock(0, T{}, "0");
        chain.push_back(genesisBlock);
    }

    // Метод для добавления нового блока
    void addBlock(const T& data) {
        size_t newIndex = chain.size();
        std::string prevHash = chain.back().hash;
        Block<T> newBlock(newIndex, data, prevHash);
        chain.push_back(newBlock);
    }

//     Метод для вывода всей цепочки блоков
//    void printChain() const {
//        for (const auto& block : chain) {
//            std::cout << "Index: " << block.index << "\n"
//                      << "Data: " << block.data << "\n"
//                      << "Prev Hash: " << block.prevHash << "\n"
//                      << "Hash: " << block.hash << "\n"
//                      << "Timestamp: " << std::asctime(std::localtime(&block.timestamp)) // Вывод времени
//                      << "-------------------------\n";
//        }
//    }

    // Метод для проверки целостности цепочки
    bool isValid() const {
        for (size_t i = 1; i < chain.size(); ++i) {
            const Block<T>& currentBlock = chain[i];
            const Block<T>& previousBlock = chain[i - 1];

            // Проверяем, совпадает ли хеш текущего блока с вычисленным хешем
            if (!currentBlock.isHashValid()) {
                return false;
            }

            // Проверяем, совпадает ли хеш предыдущего блока
            if (currentBlock.prevHash != previousBlock.hash) {
                return false;
            }
        }
        return true;
    }
};