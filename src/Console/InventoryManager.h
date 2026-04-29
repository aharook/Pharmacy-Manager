#ifndef CONSOLE_INVENTORY_MANAGER_H
#define CONSOLE_INVENTORY_MANAGER_H

#include <memory>
#include <string>
#include "InventoryService.h"

class InventoryManager {
private:
    std::unique_ptr<InventoryService> inventoryService_;

public:
    explicit InventoryManager(const std::string& productsFilePath = "pharmacy_products.txt");

    void run();

private:
    void viewProducts();
    void createProduct();
};

#endif
