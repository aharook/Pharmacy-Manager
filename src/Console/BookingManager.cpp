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

    BookingResult result = bookingService_->createBooking(orderId);

    if (!result.success) {
        ConsoleUIHelper::displayError(result.message);
        return;
    }

    ConsoleUIHelper::displayMessage(result.message);
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

    double penaltyAmount = 0.0;
    BookingResult result = bookingService_->markBookingAsMissed(orderId, penaltyAmount);

    if (!result.success) {
        ConsoleUIHelper::displayError(result.message);
        return;
    }

    std::cout << "\n✓ " << result.message << std::endl;
    std::cout << "Penalty amount (20%): " << std::fixed << std::setprecision(2) << penaltyAmount << " USD" << std::endl;
}
