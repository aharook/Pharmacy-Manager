#include "BookingService.h"

BookingService::BookingService(FileOrderRepository& orderRepository)
    : orderRepository_(orderRepository) {}

BookingResult BookingService::createBooking(const std::string& orderId) {
    BookingResult result;
    result.success = false;

    if (orderId.empty()) {
        result.message = "Order ID cannot be empty!";
        return result;
    }

    try {
        const Order& order = orderRepository_.getById(orderId);
        bookings_.emplace_back(orderId, false);
        result.success = true;
        result.message = "Booking created for order: " + orderId;
        return result;
    } catch (const std::exception& e) {
        result.message = "Order not found!";
        return result;
    }
}

const std::vector<Booking>& BookingService::getAllBookings() const {
    return bookings_;
}

BookingResult BookingService::markBookingAsMissed(const std::string& orderId, double& penaltyAmount) {
    BookingResult result;
    result.success = false;

    if (orderId.empty()) {
        result.message = "Order ID cannot be empty!";
        return result;
    }

    for (auto& booking : bookings_) {
        if (booking.getOrderId() == orderId) {
            booking.markAsMissed();
            
            try {
                const Order& order = orderRepository_.getById(orderId);
                penaltyAmount = booking.calculatePenalty(order.getTotal());
                result.success = true;
                result.message = "Booking marked as missed!";
                return result;
            } catch (...) {
                result.message = "Could not calculate penalty!";
                return result;
            }
        }
    }

    result.message = "Booking not found!";
    return result;
}
