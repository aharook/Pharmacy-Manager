#ifndef CONSOLE_PHARMACY_CONSOLE_H
#define CONSOLE_PHARMACY_CONSOLE_H

#include <string>
#include <vector>

#include "../Domain/Booking.h"
#include "../Domain/Order.h"
#include "../Domain/Product.h"
#include "../Infrastructure/FileOrderRepository.h"

class PharmacyConsole {
private:
    std::vector<Product> products_;
    FileOrderRepository& orderRepository_;
    std::vector<Booking> bookings_;

public:
    explicit PharmacyConsole(FileOrderRepository& orderRepository);

    void run();

private:
    void showMainMenu();
    void showInventoryMenu();
    void showOrderMenu();
    void showBookingMenu();

    void viewProducts();
    void createOrder();
    void viewOrders();

    void createBooking();
    void viewBookings();
    void markBookingAsMissed();

    int getUserChoice();
    std::string getUserInput(const std::string& prompt);
    void displayMessage(const std::string& msg);
    void displayError(const std::string& error);
};

#endif
