#ifndef APPLICATION_INVENTORY_SERVICE_H
#define APPLICATION_INVENTORY_SERVICE_H

#include <memory>
#include <string>
#include <vector>
#include "../Domain/Product.h"
#include "../Infrastructure/FileProductRepository.h"

class InventoryService {
private:
    std::unique_ptr<FileProductRepository> productRepository_;

public:
    explicit InventoryService(const std::string& productsFilePath = "pharmacy_products.txt");

    const std::vector<Product>& getAllProducts() const;

    void addProduct(const std::string& name, int quantity, double price);

    Product* findProductByName(const std::string& name);
    const Product* findProductByName(const std::string& name) const;

    void updateProduct(const Product& product);
};

#endif
