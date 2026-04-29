#ifndef CONSOLE_BOOKING_SERVICE_H
#define CONSOLE_BOOKING_SERVICE_H

#include <string>
#include <vector>
#include "../Domain/Booking.h"
#include "../Infrastructure/FileOrderRepository.h"

struct BookingResult {
    bool success;
    std::string message;
};

class BookingService {
private:
    FileOrderRepository& orderRepository_;
    std::vector<Booking> bookings_;

public:
    explicit BookingService(FileOrderRepository& orderRepository);


    BookingResult createBooking(const std::string& orderId);

    const std::vector<Booking>& getAllBookings() const;

    BookingResult markBookingAsMissed(const std::string& orderId, double& penaltyAmount);
};

#endif
