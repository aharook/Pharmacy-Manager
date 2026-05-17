#include "OrderManager.h"

#include <iostream>
#include <iomanip>

OrderManager::OrderManager(IOrderRepository& orderRepository, IProductRepository& productRepository) {
    orderService_ = std::make_unique<OrderService>(orderRepository, productRepository);
}

void OrderManager::run() {
    bool inMenu = true;

    while (inMenu) {
        std::cout << "\n===== ORDERS =====" << std::endl;
        std::cout << "1. Create order" << std::endl;
        std::cout << "2. View orders" << std::endl;
        std::cout << "3. Mark order as received (delete)" << std::endl;
        std::cout << "0. Back" << std::endl;
        std::cout << "Choose option: ";

        int choice = getUserChoice();

        switch (choice) {
            case 1:
                createOrder();
                break;
            case 2:
                viewOrders();
                break;
            case 3:
                markOrderAsReceived();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Invalid choice.");
        }
    }
}

void OrderManager::createOrder() {
    std::cout << "\n--- Create Order ---" << std::endl;

    std::string productName = getUserInput("Enter product name: ");
    std::string quantityStr = getUserInput("Enter quantity: ");
    std::string saleTypeStr = getUserInput("Sale type (1=Direct, 2=Booking): ");

    try {
        int quantity = std::stoi(quantityStr);
        int saleType = std::stoi(saleTypeStr);

        OrderResult result = orderService_->createOrder(productName, quantity, saleType);

        std::cout << "\nOrder created successfully!" << std::endl;
        std::cout << "ID: " << result.orderId << std::endl;
        std::cout << "Total: " << std::fixed << std::setprecision(2) << result.total << " USD" << std::endl;
    } catch (const std::invalid_argument& e) {
        displayError(e.what());
    } catch (const std::runtime_error& e) {
        displayError(e.what());
    } catch (const std::exception& e) {
        displayError(std::string("Invalid input: ") + e.what());
    }
}

void OrderManager::viewOrders() {
    auto allOrders = orderService_->getAllOrders();

    if (allOrders.empty()) {
        displayMessage("No orders.");
        return;
    }

    std::cout << "\n--- Orders List ---" << std::endl;
    std::cout << std::setw(15) << "ID"
              << std::setw(20) << "Type"
              << std::setw(15) << "Total"
              << std::setw(15) << "Status" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for (const auto& order : allOrders) {
        std::string saleType = (order.getSaleType() == SaleType::DIRECT) ? "Direct" : "Booking";
        std::string status = order.isReceived() ? "Received" : "Pending";
        std::cout << std::setw(15) << order.getId()
                  << std::setw(20) << saleType
                  << std::setw(15) << std::fixed << std::setprecision(2)
                  << order.getTotal()
                  << std::setw(15) << status << std::endl;
    }
}

void OrderManager::markOrderAsReceived() {
    std::cout << "\n--- Mark Order as Received ---" << std::endl;

    auto allOrders = orderService_->getAllOrders();

    if (allOrders.empty()) {
        displayMessage("No orders to mark as received.");
        return;
    }

    std::cout << "\nPending orders:" << std::endl;
    std::cout << std::setw(15) << "ID"
              << std::setw(20) << "Type"
              << std::setw(15) << "Total" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for (const auto& order : allOrders) {
        if (!order.isReceived()) {
            std::string saleType = (order.getSaleType() == SaleType::DIRECT) ? "Direct" : "Booking";
            std::cout << std::setw(15) << order.getId()
                      << std::setw(20) << saleType
                      << std::setw(15) << std::fixed << std::setprecision(2)
                      << order.getTotal() << std::endl;
        }
    }

    std::string orderId = getUserInput("Enter order ID to mark as received: ");

    if (orderId.empty()) {
        displayError("Order ID cannot be empty!");
        return;
    }

    try {
        orderService_->markOrderAsReceivedAndDelete(orderId);
        displayMessage("Order marked as received and deleted successfully!");
    } catch (const std::runtime_error& e) {
        displayError(e.what());
    } catch (const std::exception& e) {
        displayError(std::string("Error: ") + e.what());
    }
}

int OrderManager::getUserChoice() {
    std::string input;
    std::getline(std::cin, input);
    try {
        return std::stoi(input);
    } catch (...) {
        return -1;
    }
}

std::string OrderManager::getUserInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void OrderManager::displayMessage(const std::string& msg) {
    std::cout << "\nSuccess: " << msg << std::endl;
}

void OrderManager::displayError(const std::string& error) {
    std::cout << "\nError: " << error << std::endl;
}
