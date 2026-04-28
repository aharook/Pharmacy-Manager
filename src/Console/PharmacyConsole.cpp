#include "PharmacyConsole.h"

#include <iostream>
#include <iomanip>
#include <sstream>

PharmacyConsole::PharmacyConsole(FileOrderRepository& orderRepository)
    : orderRepository_(orderRepository) {
    products_.emplace_back("Aspirin", 50, 5.0);
    products_.emplace_back("Vitamin C", 30, 3.5);
    products_.emplace_back("Ibuprofen", 25, 4.0);
    products_.emplace_back("Paracetamol", 40, 4.5);
    products_.emplace_back("Analgin", 40, 4.5);
    products_.emplace_back("Doctrin", 40, 4.5);
}

void PharmacyConsole::run() {
    bool isRunning = true;

    while (isRunning) {
        showMainMenu();
        int choice = getUserChoice();

        switch (choice) {
            case 1:
                showInventoryMenu();
                break;
            case 2:
                showOrderMenu();
                break;
            case 3:
                showBookingMenu();
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

void PharmacyConsole::showMainMenu() {
    std::cout << "\n========== PHARMACY MANAGER (SIMPLIFIED) ==========" << std::endl;
    std::cout << "1. Inventory Management" << std::endl;
    std::cout << "2. Orders" << std::endl;
    std::cout << "3. Bookings" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Choose option: ";
}

void PharmacyConsole::showInventoryMenu() {
    bool inMenu = true;

    while (inMenu) {
        std::cout << "\n===== INVENTORY MANAGEMENT =====" << std::endl;
        std::cout << "1. View all products" << std::endl;
        std::cout << "0. Back" << std::endl;
        std::cout << "Choose option: ";

        int choice = getUserChoice();

        switch (choice) {
            case 1:
                viewProducts();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Invalid choice.");
        }
    }
}

void PharmacyConsole::viewProducts() {
    std::cout << "\n--- Inventory Status ---" << std::endl;
    std::cout << std::setw(20) << "Product Name" 
              << std::setw(15) << "Quantity"
              << std::setw(15) << "Price" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for (const auto& product : products_) {
        std::cout << std::setw(20) << product.getName()
                  << std::setw(15) << product.getPackCount()
                  << std::setw(15) << std::fixed << std::setprecision(2) 
                  << product.getPrice() << std::endl;
    }
}

void PharmacyConsole::showOrderMenu() {
    bool inMenu = true;

    while (inMenu) {
        std::cout << "\n===== ORDERS =====" << std::endl;
        std::cout << "1. Create order" << std::endl;
        std::cout << "2. View orders" << std::endl;
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
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Invalid choice.");
        }
    }
}

void PharmacyConsole::createOrder() {
    std::cout << "\n--- Create Order ---" << std::endl;

    std::string productName = getUserInput("Enter product name: ");
    std::string quantityStr = getUserInput("Enter quantity: ");
    std::string saleTypeStr = getUserInput("Sale type (1=Direct, 2=Booking): ");

    try {
        int quantity = std::stoi(quantityStr);
        int saleTypeChoice = std::stoi(saleTypeStr);

        Product* selectedProduct = nullptr;
        for (auto& p : products_) {
            if (p.getName() == productName) {
                selectedProduct = &p;
                break;
            }
        }

        if (!selectedProduct) {
            displayError("Product not found!");
            return;
        }

        if (selectedProduct->getPackCount() < quantity) {
            displayError("Insufficient stock!");
            return;
        }

        SaleType saleType = (saleTypeChoice == 2) ? SaleType::BOOKING : SaleType::DIRECT;
        
        std::string orderId = "ORD" + std::to_string(orderRepository_.getAll().size() + 1);
        Order order(orderId, saleType);
        order.addItem(OrderItem(*selectedProduct, quantity));
        order.calculateTotal();

        orderRepository_.save(order);
        selectedProduct->decreasePackCount(quantity);

        std::cout << "\nOrder created successfully!" << std::endl;
        std::cout << "ID: " << orderId << std::endl;
        std::cout << "Total: " << std::fixed << std::setprecision(2) << order.getTotal() << " USD" << std::endl;
    } catch (const std::exception& e) {
        displayError(std::string("Error: ") + e.what());
    }
}

void PharmacyConsole::viewOrders() {
    auto allOrders = orderRepository_.getAll();

    if (allOrders.empty()) {
        displayMessage("No orders.");
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

void PharmacyConsole::showBookingMenu() {
    bool inMenu = true;

    while (inMenu) {
        std::cout << "\n===== BOOKINGS =====" << std::endl;
        std::cout << "1. Create booking" << std::endl;
        std::cout << "2. View bookings" << std::endl;
        std::cout << "3. Mark as missed" << std::endl;
        std::cout << "0. Back" << std::endl;
        std::cout << "Choose option: ";

        int choice = getUserChoice();

        switch (choice) {
            case 1:
                createBooking();
                break;
            case 2:
                viewBookings();
                break;
            case 3:
                markBookingAsMissed();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Invalid choice.");
        }
    }
}

void PharmacyConsole::createBooking() {
    std::cout << "\n--- Create Booking ---" << std::endl;

    std::string orderId = getUserInput("Enter order ID: ");

    try {
        const Order& order = orderRepository_.getById(orderId);
        bookings_.emplace_back(orderId, false);
        displayMessage("Booking created for order: " + orderId);
    } catch (const std::exception& e) {
        displayError("Order not found!");
    }
}

void PharmacyConsole::viewBookings() {
    if (bookings_.empty()) {
        displayMessage("No bookings.");
        return;
    }

    std::cout << "\n--- Bookings ---" << std::endl;
    std::cout << std::setw(15) << "Order ID"
              << std::setw(15) << "Status" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for (const auto& booking : bookings_) {
        std::string status = booking.isMissed() ? "Missed" : "Active";
        std::cout << std::setw(15) << booking.getOrderId()
                  << std::setw(15) << status << std::endl;
    }
}

void PharmacyConsole::markBookingAsMissed() {
    std::string orderId = getUserInput("Enter booking ID: ");

    for (auto& booking : bookings_) {
        if (booking.getOrderId() == orderId) {
            booking.markAsMissed();
            
            try {
                const Order& order = orderRepository_.getById(orderId);
                double penaltyAmount = booking.calculatePenalty(order.getTotal());
                std::cout << "\nBooking marked as missed!" << std::endl;
                std::cout << "Original amount: " << std::fixed << std::setprecision(2) 
                          << order.getTotal() << " USD" << std::endl;
                std::cout << "Amount with penalty (20%): " << penaltyAmount << " USD" << std::endl;
            } catch (...) {}
            return;
        }
    }

    displayError("Booking not found!");
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

std::string PharmacyConsole::getUserInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void PharmacyConsole::displayMessage(const std::string& msg) {
    std::cout << "\n✓ " << msg << std::endl;
}

void PharmacyConsole::displayError(const std::string& error) {
    std::cout << "\n✗ Error: " << error << std::endl;
}
