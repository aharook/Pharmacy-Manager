#ifndef CONSOLE_BOOKING_MANAGER_H
#define CONSOLE_BOOKING_MANAGER_H

#include <memory>
#include <string>
#include "../Application/BookingService.h"
#include "../Infrastructure/FileOrderRepository.h"

class BookingManager {
private:
    std::unique_ptr<BookingService> bookingService_;

public:
    explicit BookingManager(FileOrderRepository& orderRepository);

    void run();

private:
    void createBooking();
    void viewBookings();
    void markBookingAsMissed();
};

#endif
