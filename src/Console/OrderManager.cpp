#include "OrderManager.h"
#include "ConsoleUIHelper.h"

#include <iostream>
#include <iomanip>

OrderManager::OrderManager(FileOrderRepository& orderRepository, const std::string& productsFilePath) {
    orderService_ = std::make_unique<OrderService>(orderRepository, productsFilePath);
}

void OrderManager::run() {
    bool inMenu = true;

    while (inMenu) {
        std::cout << "\n===== ORDERS =====" << std::endl;
        std::cout << "1. Create order" << std::endl;
        std::cout << "2. View orders" << std::endl;
        std::cout << "0. Back" << std::endl;
        std::cout << "Choose option: ";

        int choice = ConsoleUIHelper::getUserChoice();

        switch (choice) {
            case 1:
                createOrder();
                break;
            case 2:
                viewOrders();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                ConsoleUIHelper::displayError("Invalid choice.");
        }
    }
}

void OrderManager::createOrder() {
    std::cout << "\n--- Create Order ---" << std::endl;

    std::string productName = ConsoleUIHelper::getUserInput("Enter product name: ");
    std::string quantityStr = ConsoleUIHelper::getUserInput("Enter quantity: ");
    std::string saleTypeStr = ConsoleUIHelper::getUserInput("Sale type (1=Direct, 2=Booking): ");

    try {
        int quantity = std::stoi(quantityStr);
        int saleType = std::stoi(saleTypeStr);

        OrderResult result = orderService_->createOrder(productName, quantity, saleType);

        if (!result.success) {
            ConsoleUIHelper::displayError(result.message);
            return;
        }

        std::cout << "\n✓ Order created successfully!" << std::endl;
        std::cout << "ID: " << result.orderId << std::endl;
        std::cout << "Total: " << std::fixed << std::setprecision(2) << result.total << " USD" << std::endl;
    } catch (const std::exception& e) {
        ConsoleUIHelper::displayError(std::string("Invalid input: ") + e.what());
    }
}

void OrderManager::viewOrders() {
    auto allOrders = orderService_->getAllOrders();

    if (allOrders.empty()) {
        ConsoleUIHelper::displayMessage("No orders.");
        return;
    }

    std::cout << "\n--- Orders List ---" << std::endl;
    std::cout << std::setw(15) << "ID" 
              << std::setw(20) << "Type"
              << std::setw(15) << "Total" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for (const auto& order : allOrders) {
        std::string saleType = (order.getSaleType() == SaleType::DIRECT) ? "Direct" : "Booking";
        std::cout << std::setw(15) << order.getId()
                  << std::setw(20) << saleType
                  << std::setw(15) << std::fixed << std::setprecision(2) 
                  << order.getTotal() << std::endl;
    }
}
