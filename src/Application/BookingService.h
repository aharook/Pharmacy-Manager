#ifndef APPLICATION_BOOKING_SERVICE_H
#define APPLICATION_BOOKING_SERVICE_H

#include <string>
#include <vector>
#include "../Domain/Booking.h"
#include "../Infrastructure/FileOrderRepository.h"

struct BookingResult {
    double penaltyAmount;
};

class BookingService {
private:
    FileOrderRepository& orderRepository_;
    std::vector<Booking> bookings_;

public:
    explicit BookingService(FileOrderRepository& orderRepository);

    void createBooking(const std::string& orderId);

    const std::vector<Booking>& getAllBookings() const;

    BookingResult markBookingAsMissed(const std::string& orderId);
};

#endif
