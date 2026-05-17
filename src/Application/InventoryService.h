#ifndef APPLICATION_INVENTORY_SERVICE_H
#define APPLICATION_INVENTORY_SERVICE_H

#include <memory>
#include <string>
#include <vector>
#include "../Domain/Product.h"
#include "../Domain/ProductRepository.h"

class InventoryService {
private:
    IProductRepository& productRepository_;

public:
    explicit InventoryService(IProductRepository& productRepository);

    const std::vector<Product>& getAllProducts() const;

    void addProduct(const std::string& name, int quantity, double price);

    Product* findProductByName(const std::string& name);
    const Product* findProductByName(const std::string& name) const;

    void updateProduct(const Product& product);
};

#endif
