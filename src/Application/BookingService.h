#ifndef APPLICATION_BOOKING_SERVICE_H
#define APPLICATION_BOOKING_SERVICE_H

#include <chrono>
#include <optional>
#include <string>
#include <uuid/uuid.h>
#include <vector>

#include "../Domain/Booking.h"
#include "../Domain/BookingRepository.h"
#include "InventoryService.h"

class BookingService {
private:
    IBookingRepository& bookingRepository_;
    InventoryService& inventoryService_;

    // Business Rule: Booking validity period (hours)
    static constexpr int BOOKING_VALIDITY_HOURS = 24;

    // Business Rule: Maximum concurrent bookings per product
    static constexpr int MAX_CONCURRENT_BOOKINGS_PER_PRODUCT = 5;

    // Generate unique booking ID
    std::string generateBookingId() const;

public:
    BookingService(IBookingRepository& bookingRepository, InventoryService& inventoryService)
        : bookingRepository_(bookingRepository), inventoryService_(inventoryService) {}

    // Create booking: reserve items for customer
    // Business Rule: Check stock, check conflicts, set expiration
    std::optional<Booking> createBooking(
        const std::string& productName,
        int quantity,
        const std::string& customerId);

    // Confirm booking: move from RESERVED to CONFIRMED
    bool confirmBooking(const std::string& bookingId);

    // Complete booking: move from CONFIRMED to COMPLETED
    bool completeBooking(const std::string& bookingId);

    // Cancel booking: return items to stock, move to CANCELLED
    // Business Rule: Release items only if booking not yet COMPLETED
    bool cancelBooking(const std::string& bookingId);

    // Get active bookings (not expired, not cancelled)
    std::vector<Booking> getActiveBookings() const;

    // Get bookings by customer
    std::vector<Booking> getBookingsByCustomer(const std::string& customerId) const;

    // Get bookings by product
    std::vector<Booking> getBookingsByProduct(const std::string& productName) const;

    // Check if bookings for product are expiring soon (next hour)
    std::vector<Booking> getExpiringBookings() const;

    // Business Rule: Clean up expired bookings
    int cleanupExpiredBookings();
};

#endif
