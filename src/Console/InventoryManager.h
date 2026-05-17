#ifndef CONSOLE_INVENTORY_MANAGER_H
#define CONSOLE_INVENTORY_MANAGER_H

#include <memory>
#include <string>
#include "../Application/InventoryService.h"
#include "../Domain/ProductRepository.h"

class InventoryManager {
private:
    std::unique_ptr<InventoryService> inventoryService_;

public:
    explicit InventoryManager(IProductRepository& productRepository);

    void run();

private:
    void viewProducts();
    void createProduct();

    std::string getUserInput(const std::string& prompt);
    void displayMessage(const std::string& msg);
    void displayError(const std::string& error);
};

#endif
