#include "BookingService.h"
#include <stdexcept>

BookingService::BookingService(FileOrderRepository& orderRepository)
    : orderRepository_(orderRepository) {}

void BookingService::createBooking(const std::string& orderId) {
    if (orderId.empty()) {
        throw std::invalid_argument("Order ID cannot be empty!");
    }

    const Order& order = orderRepository_.getById(orderId);
    bookings_.emplace_back(orderId, false);
}

const std::vector<Booking>& BookingService::getAllBookings() const {
    return bookings_;
}

BookingResult BookingService::markBookingAsMissed(const std::string& orderId) {
    if (orderId.empty()) {
        throw std::invalid_argument("Order ID cannot be empty!");
    }

    for (auto& booking : bookings_) {
        if (booking.getOrderId() == orderId) {
            booking.markAsMissed();
            const Order& order = orderRepository_.getById(orderId);
            
            BookingResult result;
            result.penaltyAmount = booking.calculatePenalty(order.getTotal());
            return result;
        }
    }

    throw std::runtime_error("Booking not found!");
}
