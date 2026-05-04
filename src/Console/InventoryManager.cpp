#include "InventoryManager.h"

#include <iostream>
#include <iomanip>

InventoryManager::InventoryManager(const std::string& productsFilePath) {
    inventoryService_ = std::make_unique<InventoryService>(productsFilePath);
}

void InventoryManager::run() {
    bool inMenu = true;

    while (inMenu) {
        std::cout << "\n===== INVENTORY MANAGEMENT =====" << std::endl;
        std::cout << "1. View all products" << std::endl;
        std::cout << "2. Create new product" << std::endl;
        std::cout << "0. Back" << std::endl;
        std::cout << "Choose option: ";

        std::string input;
        std::getline(std::cin, input);
        
        try {
            int choice = std::stoi(input);
            
            switch (choice) {
                case 1:
                    viewProducts();
                    break;
                case 2:
                    createProduct();
                    break;
                case 0:
                    inMenu = false;
                    break;
                default:
                    displayError("Invalid choice.");
            }
        } catch (...) {
            displayError("Invalid input.");
        }
    }
}

void InventoryManager::viewProducts() {
    std::cout << "\n--- Inventory Status ---" << std::endl;
    std::cout << std::setw(20) << "Product Name" 
              << std::setw(15) << "Quantity"
              << std::setw(15) << "Price" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    const auto& products = inventoryService_->getAllProducts();
    
    if (products.empty()) {
        displayMessage("No products available.");
        return;
    }

    for (const auto& product : products) {
        std::cout << std::setw(20) << product.getName()
                  << std::setw(15) << product.getPackCount()
                  << std::setw(15) << std::fixed << std::setprecision(2) 
                  << product.getPrice() << std::endl;
    }
}

void InventoryManager::createProduct() {
    std::cout << "\n--- Create New Product ---" << std::endl;

    std::string name = getUserInput("Enter product name: ");
    std::string quantityStr = getUserInput("Enter quantity: ");
    std::string priceStr = getUserInput("Enter price: ");

    try {
        int quantity = std::stoi(quantityStr);
        double price = std::stod(priceStr);
        
        inventoryService_->addProduct(name, quantity, price);

        std::cout << "\n Product created successfully!" << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Quantity: " << quantity << std::endl;
        std::cout << "Price: " << std::fixed << std::setprecision(2) << price << " USD" << std::endl;
    } catch (const std::invalid_argument& e) {
        displayError(e.what());
    } catch (const std::exception& e) {
        displayError(std::string("Invalid input: ") + e.what());
    }
}

std::string InventoryManager::getUserInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void InventoryManager::displayMessage(const std::string& msg) {
    std::cout << "\nsuccess: " << msg << std::endl;
}

void InventoryManager::displayError(const std::string& error) {
    std::cout << "\n Error: " << error << std::endl;
}
