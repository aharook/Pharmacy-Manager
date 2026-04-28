#include "BookingService.h"

#include <sstream>
#include <iomanip>

std::string BookingService::generateBookingId() const {
    // Simple ID generation using timestamp + random suffix
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    
    std::ostringstream oss;
    oss << "BK" << std::hex << millis.count();
    return oss.str();
}

std::optional<Booking> BookingService::createBooking(
    const std::string& productName,
    int quantity,
    const std::string& customerId) {
    
    // Business Rule: Validate quantity
    if (quantity <= 0) {
        return std::nullopt;
    }

    // Business Rule: Check if product exists and has stock
    if (!inventoryService_.reserveItems(productName, quantity)) {
        return std::nullopt;
    }

    // Business Rule: Check max concurrent bookings per product
    auto existingBookings = bookingRepository_.getByProductName(productName);
    int activeCount = 0;
    for (const auto& b : existingBookings) {
        if (b.isActive() && !b.isExpired()) {
            activeCount++;
        }
    }
    
    if (activeCount >= MAX_CONCURRENT_BOOKINGS_PER_PRODUCT) {
        return std::nullopt;
    }

    // Create booking with expiration
    auto expiresAt = std::chrono::system_clock::now() + 
                     std::chrono::hours(BOOKING_VALIDITY_HOURS);
    
    std::string bookingId = generateBookingId();
    Booking booking(bookingId, productName, quantity, customerId, expiresAt);
    
    bookingRepository_.save(booking);
    return booking;
}

bool BookingService::confirmBooking(const std::string& bookingId) {
    auto booking = bookingRepository_.getById(bookingId);
    if (!booking) {
        return false;
    }

    // Business Rule: Can only confirm RESERVED bookings
    if (booking->getStatus() != BookingStatus::RESERVED) {
        return false;
    }

    // Business Rule: Check if booking has expired
    if (booking->isExpired()) {
        return false;
    }

    booking->changeStatus(BookingStatus::CONFIRMED);
    bookingRepository_.update(*booking);
    return true;
}

bool BookingService::completeBooking(const std::string& bookingId) {
    auto booking = bookingRepository_.getById(bookingId);
    if (!booking) {
        return false;
    }

    // Business Rule: Can only complete CONFIRMED bookings
    if (booking->getStatus() != BookingStatus::CONFIRMED) {
        return false;
    }

    booking->changeStatus(BookingStatus::COMPLETED);
    bookingRepository_.update(*booking);
    return true;
}

bool BookingService::cancelBooking(const std::string& bookingId) {
    auto booking = bookingRepository_.getById(bookingId);
    if (!booking) {
        return false;
    }

    // Business Rule: Can only cancel if not yet COMPLETED
    if (booking->getStatus() == BookingStatus::COMPLETED) {
        return false;
    }

    // Business Rule: Release items back to inventory
    if (booking->isActive()) {
        if (!inventoryService_.releaseItems(booking->getProductName(), booking->getQuantity())) {
            return false;
        }
    }

    booking->changeStatus(BookingStatus::CANCELLED);
    bookingRepository_.update(*booking);
    return true;
}

std::vector<Booking> BookingService::getActiveBookings() const {
    return bookingRepository_.getActive();
}

std::vector<Booking> BookingService::getBookingsByCustomer(
    const std::string& customerId) const {
    return bookingRepository_.getByCustomerId(customerId);
}

std::vector<Booking> BookingService::getBookingsByProduct(
    const std::string& productName) const {
    return bookingRepository_.getByProductName(productName);
}

std::vector<Booking> BookingService::getExpiringBookings() const {
    std::vector<Booking> result;
    auto now = std::chrono::system_clock::now();
    auto oneHourLater = now + std::chrono::hours(1);

    for (const auto& booking : bookingRepository_.getActive()) {
        if (booking.getExpiresAt() <= oneHourLater) {
            result.push_back(booking);
        }
    }
    return result;
}

int BookingService::cleanupExpiredBookings() {
    int cleaned = 0;
    auto allBookings = bookingRepository_.getAll();
    
    for (const auto& booking : allBookings) {
        if (booking.isExpired() && booking.isActive()) {
            if (cancelBooking(booking.getId())) {
                cleaned++;
            }
        }
    }

    return cleaned;
}
