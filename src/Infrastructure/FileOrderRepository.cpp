#include "FileOrderRepository.h"

#include <fstream>
#include <sstream>

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
        file << id << "|" << static_cast<int>(order.getSaleType()) << "|" 
             << order.getTotal() << "\n";
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
        
        std::stringstream ss(line);
        std::string id, saleTypeStr, totalStr;
        
        if (std::getline(ss, id, '|') && 
            std::getline(ss, saleTypeStr, '|') && 
            std::getline(ss, totalStr, '|')) {
            
            int saleTypeInt = std::stoi(saleTypeStr);
            double total = std::stod(totalStr);
            
            SaleType saleType = (saleTypeInt == 0) ? SaleType::DIRECT : SaleType::BOOKING;
            Order order(id, saleType);
            order.setTotal(total);
            cache_[id] = order;
        }
    }
    file.close();
}

