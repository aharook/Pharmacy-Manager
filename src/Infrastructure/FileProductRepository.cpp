#include "FileProductRepository.h"
#include <fstream>
#include <sstream>
#include <algorithm>

FileProductRepository::FileProductRepository(const std::string& filePath)
    : filePath_(filePath),
      serializer_(std::make_unique<ProductSerializer>()),
      deserializer_(std::make_unique<ProductDeserializer>()) {
    loadFromFile();
}

FileProductRepository::~FileProductRepository() {
    saveToFile();
}

void FileProductRepository::save(const Product& product) {
    auto it = std::find_if(cache_.begin(), cache_.end(),
        [&product](const Product& p) { return p.getName() == product.getName(); });

    if (it != cache_.end()) {
        *it = product;
    } else {
        cache_.push_back(product);
    }
    saveToFile();
}

const std::vector<Product>& FileProductRepository::getAll() const {
    return cache_;
}

Product* FileProductRepository::findByName(const std::string& name) {
    for (auto& product : cache_) {
        if (product.getName() == name) {
            return &product;
        }
    }
    return nullptr;
}

const Product* FileProductRepository::findByName(const std::string& name) const {
    for (const auto& product : cache_) {
        if (product.getName() == name) {
            return &product;
        }
    }
    return nullptr;
}

void FileProductRepository::saveToFile() {
    std::ofstream file(filePath_);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath_);
    }

    for (const auto& product : cache_) {
        file << serializer_->serialize(product) << "\n";
    }
    file.close();
}

void FileProductRepository::loadFromFile() {
    std::ifstream file(filePath_);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        try {
            Product product = deserializer_->deserialize(line);
            cache_.push_back(product);
        } catch (...) {
            continue;
        }
    }
    file.close();
}
