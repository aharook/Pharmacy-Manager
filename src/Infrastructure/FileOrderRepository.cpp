#include "FileOrderRepository.h"
#include <fstream>
#include <sstream>

FileOrderRepository::FileOrderRepository(const std::string& filePath)
    : filePath_(filePath),
      serializer_(std::make_unique<OrderSerializer>()),
      deserializer_(std::make_unique<OrderDeserializer>()) {
    loadFromFile();
}

FileOrderRepository::~FileOrderRepository() {
    saveToFile();
}

void FileOrderRepository::save(const Order& order) {
    cache_[order.getId()] = order;
    saveToFile();
}

const Order& FileOrderRepository::getById(const std::string& id) const {
    auto it = cache_.find(id);
    if (it == cache_.end()) {
        throw std::runtime_error("Order not found: " + id);
    }
    return it->second;
}

std::vector<Order> FileOrderRepository::getAll() const {
    std::vector<Order> result;
    for (const auto& [id, order] : cache_) {
        result.push_back(order);
    }
    return result;
}

void FileOrderRepository::update(const Order& order) {
    cache_[order.getId()] = order;
    saveToFile();
}

void FileOrderRepository::deleteById(const std::string& id) {
    cache_.erase(id);
    saveToFile();
}

void FileOrderRepository::saveToFile() {
    std::ofstream file(filePath_);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath_);
    }

    for (const auto& [id, order] : cache_) {
        file << serializer_->serialize(order) << "\n";
    }
    file.close();
}

void FileOrderRepository::loadFromFile() {
    std::ifstream file(filePath_);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        try {
            Order order = deserializer_->deserialize(line);
            cache_[order.getId()] = order;
        } catch (...) {
            continue;
        }
    }
    file.close();
}

