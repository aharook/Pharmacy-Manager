#include "InventoryService.h"
#include <stdexcept>

InventoryService::InventoryService(const std::string& productsFilePath) {
    productRepository_ = std::make_unique<FileProductRepository>(productsFilePath);
}

const std::vector<Product>& InventoryService::getAllProducts() const {
    return productRepository_->getAll();
}

void InventoryService::addProduct(const std::string& name, int quantity, double price) {
    if (name.empty()) {
        throw std::invalid_argument("Product name cannot be empty!");
    }

    if (productRepository_->findByName(name) != nullptr) {
        throw std::invalid_argument("Product with this name already exists!");
    }

    if (quantity < 0) {
        throw std::invalid_argument("Quantity must be non-negative!");
    }

    if (price < 0) {
        throw std::invalid_argument("Price must be non-negative!");
    }

    Product newProduct(name, quantity, price);
    productRepository_->save(newProduct);
}

Product* InventoryService::findProductByName(const std::string& name) {
    return productRepository_->findByName(name);
}

const Product* InventoryService::findProductByName(const std::string& name) const {
    return productRepository_->findByName(name);
}

void InventoryService::updateProduct(const Product& product) {
    productRepository_->save(product);
}
