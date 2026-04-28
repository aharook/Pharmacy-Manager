#ifndef DOMAIN_REPOSITORY_PATTERN_H
#define DOMAIN_REPOSITORY_PATTERN_H

#include <optional>
#include <string>
#include <unordered_map>

#include "Product.h"

class IInventoryRepository {
public:
    virtual ~IInventoryRepository() = default;

    virtual std::optional<Product> getByName(const std::string& name) const = 0;
    virtual void save(const Product& product) = 0;
    virtual void update(const Product& product) = 0;
};

class InMemoryInventoryRepository final : public IInventoryRepository {
    
private:
    std::unordered_map<std::string, Product> storage_;
    
public:
    std::optional<Product> getByName(const std::string& name) const override {
        auto it = storage_.find(name);
        if (it == storage_.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    void save(const Product& product) override {
        storage_.erase(product.getName());
        storage_.emplace(product.getName(), product);
    }

    void update(const Product& product) override {
        storage_.erase(product.getName());
        storage_.emplace(product.getName(), product);
    }

};


#endif
