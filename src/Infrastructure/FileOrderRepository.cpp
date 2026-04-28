#include "FileOrderRepository.h"

#include <chrono>
#include <sstream>

#include "FileStorageHelper.h"

void FileOrderRepository::save(const Order& order) {
    // Make a mutable copy since map requires non-const
    Order orderCopy = order;
    cache_[order.getId()] = orderCopy;
    isDirty_ = true;
    saveToFile();
}

std::optional<Order> FileOrderRepository::getById(const std::string& id) const {
    auto it = cache_.find(id);
    if (it == cache_.end()) {
        return std::nullopt;
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

std::vector<Order> FileOrderRepository::getByStatus(OrderStatus status) const {
    std::vector<Order> result;
    for (const auto& [id, order] : cache_) {
        if (order.getStatus() == status) {
            result.push_back(order);
        }
    }
    return result;
}

void FileOrderRepository::update(const Order& order) {
    Order orderCopy = order;
    cache_[order.getId()] = orderCopy;
    isDirty_ = true;
    saveToFile();
}

void FileOrderRepository::deleteById(const std::string& id) {
    if (cache_.erase(id) > 0) {
        isDirty_ = true;
        saveToFile();
    }
}

void FileOrderRepository::saveToFile() {
    JsonBuilder arrayBuilder(true);
    
    for (const auto& [id, order] : cache_) {
        JsonBuilder itemBuilder;
        itemBuilder.addString("id", order.getId());
        itemBuilder.addNumber("total", order.getTotal());
        itemBuilder.addString("status", orderStatusToString(order.getStatus()));
        itemBuilder.addNumber("createdAt", static_cast<long long>(order.getCreatedAt().time_since_epoch().count()));
        
        // Serialize items
        JsonBuilder itemsArray(true);
        for (const auto& item : order.getItems()) {
            itemsArray.addElement(JsonBuilder()
                .addString("productName", item.getProduct().getName())
                .addInt("quantity", item.getQuantity())
                .addNumber("unitPrice", item.getUnitPrice()));
        }
        itemBuilder.addElement(JsonBuilder()
            .addString("items", itemsArray.build()));
        
        arrayBuilder.addElement(itemBuilder);
    }

    std::string json = arrayBuilder.build();
    
    try {
        FileStorageHelper::writeFile(filePath_, json);
        isDirty_ = false;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save orders to file: " + std::string(e.what()));
    }
}

void FileOrderRepository::loadFromFile() {
    cache_.clear();
    
    if (!FileStorageHelper::fileExists(filePath_)) {
        return;
    }

    try {
        std::string content = FileStorageHelper::readFile(filePath_);
        
        if (content.empty()) {
            return;
        }

        auto items = JsonValue::parseArray(content);
        for (const auto& item : items) {
            std::string id = item.getString("id");
            double total = item.getDouble("total", 0.0);
            OrderStatus status = stringToOrderStatus(item.getString("status", "PENDING"));
            long long timestamp = item.getLongLong("createdAt", 0);
            auto createdAt = std::chrono::system_clock::time_point(
                std::chrono::system_clock::duration(timestamp));

            if (!id.empty()) {
                Order order(id, {}, total, status, createdAt);
                // Note: Items are not fully deserialized in this simplified version
                // In production, this should be completed
                cache_[id] = order;
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load orders from file: " + std::string(e.what()));
    }
}

std::string FileOrderRepository::serializeOrder(const Order& order) const {
    JsonBuilder builder;
    builder.addString("id", order.getId());
    builder.addNumber("total", order.getTotal());
    builder.addString("status", orderStatusToString(order.getStatus()));
    return builder.build();
}

Order FileOrderRepository::deserializeOrder(const std::string& json) const {
    JsonValue value = JsonValue::parse(json);
    std::string id = value.getString("id");
    double total = value.getDouble("total", 0.0);
    OrderStatus status = stringToOrderStatus(value.getString("status"));
    
    return Order(id, {}, total, status, std::chrono::system_clock::now());
}
