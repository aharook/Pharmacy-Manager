#ifndef INFRASTRUCTURE_FILE_PRODUCT_REPOSITORY_H
#define INFRASTRUCTURE_FILE_PRODUCT_REPOSITORY_H

#include <string>
#include <vector>
#include "../Domain/Product.h"

class FileProductRepository {
private:
    std::string filePath_;
    std::vector<Product> cache_;

public:
    explicit FileProductRepository(const std::string& filePath)
        : filePath_(filePath) {
        loadFromFile();
    }

    ~FileProductRepository() {
        saveToFile();
    }

    void save(const Product& product);
    const std::vector<Product>& getAll() const;
    Product* findByName(const std::string& name);
    const Product* findByName(const std::string& name) const;

    void saveToFile();
    void loadFromFile();
};

#endif
