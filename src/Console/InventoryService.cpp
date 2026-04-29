#include "InventoryService.h"

InventoryService::InventoryService(const std::string& productsFilePath) {
    productRepository_ = std::make_unique<FileProductRepository>(productsFilePath);
}

const std::vector<Product>& InventoryService::getAllProducts() const {
    return productRepository_->getAll();
}

std::string InventoryService::addProduct(const std::string& name, int quantity, double price) {
    if (name.empty()) {
        return "Product name cannot be empty!";
    }

    if (productRepository_->findByName(name) != nullptr) {
        return "Product with this name already exists!";
    }

    if (quantity < 0) {
        return "Quantity must be non-negative!";
    }

    if (price < 0) {
        return "Price must be non-negative!";
    }

    try {
        Product newProduct(name, quantity, price);
        productRepository_->save(newProduct);
        return "";  // Empty string = success
    } catch (const std::exception& e) {
        return std::string("Error: ") + e.what();
    }
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
