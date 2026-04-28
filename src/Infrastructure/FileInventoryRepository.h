#ifndef INFRASTRUCTURE_FILE_INVENTORY_REPOSITORY_H
#define INFRASTRUCTURE_FILE_INVENTORY_REPOSITORY_H

#include <memory>
#include <string>
#include <unordered_map>

#include "../Domain/RepositoryPattern.h"

class FileInventoryRepository : public IInventoryRepository {
private:
    std::string filePath_;
    std::unordered_map<std::string, Product> cache_;
    bool isDirty_;

public:
    explicit FileInventoryRepository(const std::string& filePath)
        : filePath_(filePath), isDirty_(false) {
        loadFromFile();
    }

    ~FileInventoryRepository() {
        if (isDirty_) {
            try {
                saveToFile();
            } catch (...) {
                // Error handling on destruction
            }
        }
    }

    std::optional<Product> getByName(const std::string& name) const override;
    void save(const Product& product) override;
    void update(const Product& product) override;

    // Synchronous persistence operations
    void saveToFile();
    void loadFromFile();

private:
    std::string serializeProduct(const Product& product) const;
    Product deserializeProduct(const std::string& json) const;
};

#endif
