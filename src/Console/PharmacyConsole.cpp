#include "PharmacyConsole.h"

#include <iostream>

PharmacyConsole::PharmacyConsole(FileOrderRepository& orderRepository, const std::string& productsFilePath) 
{
    inventoryManager_ = std::make_unique<InventoryManager>(productsFilePath);
    orderManager_ = std::make_unique<OrderManager>(orderRepository, productsFilePath);
    bookingManager_ = std::make_unique<BookingManager>(orderRepository);
}

void PharmacyConsole::run() {
    bool isRunning = true;

    while (isRunning) {
        std::cout << "\n========== PHARMACY MANAGER  ==========" << std::endl;
        std::cout << "1. Inventory Management" << std::endl;
        std::cout << "2. Orders" << std::endl;
        std::cout << "3. Bookings" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Choose option: ";

        int choice = getUserChoice();

        switch (choice) {
            case 1:
                inventoryManager_->run();
                break;
            case 2:
                orderManager_->run();
                break;
            case 3:
                bookingManager_->run();
                break;
            case 0:
                isRunning = false;
                displayMessage("\nThank you for using Pharmacy Manager!");
                break;
            default:
                displayError("Invalid choice.");
        }
    }
}

int PharmacyConsole::getUserChoice() {
    std::string input;
    std::getline(std::cin, input);
    try {
        return std::stoi(input);
    } catch (...) {
        return -1;
    }
}

void PharmacyConsole::displayMessage(const std::string& msg) 
{
    std::cout << "\n✓ " << msg << std::endl;
}

void PharmacyConsole::displayError(const std::string& error) 
{
    std::cout << "\n✗ Error: " << error << std::endl;
}
