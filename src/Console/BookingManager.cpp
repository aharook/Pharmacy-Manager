#include "BookingManager.h"
#include "ConsoleUIHelper.h"

#include <iostream>
#include <iomanip>

BookingManager::BookingManager(FileOrderRepository& orderRepository) {
    bookingService_ = std::make_unique<BookingService>(orderRepository);
}

void BookingManager::run() {
    bool inMenu = true;

    while (inMenu) {
        std::cout << "\n===== BOOKINGS =====" << std::endl;
        std::cout << "1. Create booking" << std::endl;
        std::cout << "2. View bookings" << std::endl;
        std::cout << "3. Mark as missed" << std::endl;
        std::cout << "0. Back" << std::endl;
        std::cout << "Choose option: ";

        int choice = ConsoleUIHelper::getUserChoice();

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
                ConsoleUIHelper::displayError("Invalid choice.");
        }
    }
}

void BookingManager::createBooking() {
    std::cout << "\n--- Create Booking ---" << std::endl;

    std::string orderId = ConsoleUIHelper::getUserInput("Enter order ID: ");

    try {
        bookingService_->createBooking(orderId);
        displayMessage("Booking created for order: " + orderId);
    } catch (const std::invalid_argument& e) {
        displayError(e.what());
    } catch (const std::runtime_error& e) {
        displayError(e.what());
    } catch (const std::exception& e) {
        displayError(std::string("Error: ") + e.what());
    }
}

void BookingManager::viewBookings() {
    const auto& bookings = bookingService_->getAllBookings();
    
    if (bookings.empty()) {
        ConsoleUIHelper::displayMessage("No bookings.");
        return;
    }

    std::cout << "\n--- Bookings ---" << std::endl;
    std::cout << std::setw(15) << "Order ID" << std::setw(15) << "Status" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;

    for (const auto& booking : bookings) {
        std::string status = booking.isMissed() ? "Missed" : "Active";
        std::cout << std::setw(15) << booking.getOrderId() << std::setw(15) << status << std::endl;
    }
}

void BookingManager::markBookingAsMissed() {
    std::string orderId = ConsoleUIHelper::getUserInput("Enter booking ID: ");

    try {
        BookingResult result = bookingService_->markBookingAsMissed(orderId);
        std::cout << "\n Booking marked as missed!" << std::endl;
        std::cout << "Penalty amount (20%): " << std::fixed << std::setprecision(2) << result.penaltyAmount << " USD" << std::endl;
    } catch (const std::invalid_argument& e) {
        displayError(e.what());
    } catch (const std::runtime_error& e) {
        displayError(e.what());
    } catch (const std::exception& e) {
        displayError(std::string("Error: ") + e.what());
    }
}

int BookingManager::getUserChoice() {
    std::string input;
    std::getline(std::cin, input);
    try {
        return std::stoi(input);
    } catch (...) {
        return -1;
    }
}

std::string BookingManager::getUserInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void BookingManager::displayMessage(const std::string& msg) {
    std::cout << "\nsuccess: " << msg << std::endl;
}

void BookingManager::displayError(const std::string& error) {
    std::cout << "\n Error: " << error << std::endl;
}
