#include "FileInventoryRepository.h"

#include <fstream>
#include <sstream>

#include "FileStorageHelper.h"

std::optional<Product> FileInventoryRepository::getByName(const std::string& name) const {
    auto it = cache_.find(name);
    if (it == cache_.end()) {
        return std::nullopt;
    }
    return it->second;
}

void FileInventoryRepository::save(const Product& product) {
    cache_[product.getName()] = product;
    isDirty_ = true;
    saveToFile();
}

void FileInventoryRepository::update(const Product& product) {
    cache_[product.getName()] = product;
    isDirty_ = true;
    saveToFile();
}

std::string FileInventoryRepository::serializeProduct(const Product& product) const {
    JsonBuilder builder;
    builder.addString("name", product.getName());
    builder.addInt("packCount", product.getPackCount());
    builder.addNumber("price", product.getPrice());
    return builder.build();
}

Product FileInventoryRepository::deserializeProduct(const std::string& json) const {
    JsonValue value = JsonValue::parse(json);
    std::string name = value.getString("name");
    int packCount = value.getInt("packCount", 0);
    double price = value.getDouble("price", 0.0);
    return Product(name, packCount, price);
}

void FileInventoryRepository::saveToFile() {
    JsonBuilder arrayBuilder(true);
    
    for (const auto& [name, product] : cache_) {
        arrayBuilder.addElement(JsonBuilder()
            .addString("name", product.getName())
            .addInt("packCount", product.getPackCount())
            .addNumber("price", product.getPrice()));
    }

    std::string json = arrayBuilder.build();
    
    try {
        FileStorageHelper::writeFile(filePath_, json);
        isDirty_ = false;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save inventory to file: " + std::string(e.what()));
    }
}

void FileInventoryRepository::loadFromFile() {
    cache_.clear();
    
    if (!FileStorageHelper::fileExists(filePath_)) {
        return; // File doesn't exist, start with empty inventory
    }

    try {
        std::string content = FileStorageHelper::readFile(filePath_);
        
        if (content.empty()) {
            return;
        }

        auto items = JsonValue::parseArray(content);
        for (const auto& item : items) {
            std::string name = item.getString("name");
            int packCount = item.getInt("packCount", 0);
            double price = item.getDouble("price", 0.0);
            
            if (!name.empty()) {
                Product product(name, packCount, price);
                cache_[name] = product;
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load inventory from file: " + std::string(e.what()));
    }
}
