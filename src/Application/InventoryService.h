#ifndef APPLICATION_INVENTORY_SERVICE_H
#define APPLICATION_INVENTORY_SERVICE_H

#include <optional>
#include <string>

#include "../Domain/Product.h"
#include "../Domain/RepositoryPattern.h"

class InventoryService {
private:
    IInventoryRepository& repository_;
public:
    explicit InventoryService(IInventoryRepository& repository)
        : repository_(repository) {}

    std::optional<Product> findProduct(const std::string& name) const {
        return repository_.getByName(name);
    }

    bool reserveItems(const std::string& name, int quantity) const {
        if (quantity <= 0) {
            return false;
        }
        auto product = repository_.getByName(name);
        if (!product) {
            return false;
        }
        return product->getPackCount() >= quantity;
    }

    bool releaseItems(const std::string& name, int quantity) {
        if (quantity <= 0) {
            return false;
        }
        auto product = repository_.getByName(name);
        if (!product) {
            return false;
        }
        product->increasePackCount(quantity);
        repository_.update(*product);
        return true;
    }

    bool decreaseStock(const std::string& name, int quantity) {
        if (!reserveItems(name, quantity)) {
            return false;
        }
        auto product = repository_.getByName(name);
        if (!product) {
            return false;
        }
        product->decreasePackCount(quantity);
        repository_.update(*product);
        return true;
    }
};

#endif
