#ifndef INFRASTRUCTURE_FILE_PRODUCT_REPOSITORY_H
#define INFRASTRUCTURE_FILE_PRODUCT_REPOSITORY_H

#include <string>
#include <vector>
#include <memory>
#include "../Domain/ProductRepository.h"
#include "ProductSerializer.h"
#include "ProductDeserializer.h"

class FileProductRepository : public IProductRepository {
private:
    std::string filePath_;
    std::vector<Product> cache_;
    std::unique_ptr<ProductSerializer> serializer_;
    std::unique_ptr<ProductDeserializer> deserializer_;

public:
    explicit FileProductRepository(const std::string& filePath);

    ~FileProductRepository();

    void save(const Product& product) override;
    const std::vector<Product>& getAll() const override;
    Product* findByName(const std::string& name) override;
    const Product* findByName(const std::string& name) const override;

private:
    void saveToFile();
    void loadFromFile();
};

#endif
