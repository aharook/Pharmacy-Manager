#ifndef DOMAIN_BOOKING_H
#define DOMAIN_BOOKING_H

#include <chrono>
#include <string>
#include <utility>

#include "BookingStatus.h"
#include "Product.h"

class Booking {
private:
    std::string id_;
    std::string productName_;
    int quantity_;
    std::string customerId_;
    BookingStatus status_;
    std::chrono::system_clock::time_point createdAt_;
    std::chrono::system_clock::time_point expiresAt_;

public:
    Booking(std::string id, std::string productName, int quantity,
            std::string customerId,
            std::chrono::system_clock::time_point expiresAt)
        : id_(std::move(id)), 
          productName_(std::move(productName)),
          quantity_(quantity),
          customerId_(std::move(customerId)),
          status_(BookingStatus::RESERVED),
          createdAt_(std::chrono::system_clock::now()),
          expiresAt_(expiresAt) {
        if (quantity <= 0) {
            throw std::invalid_argument("quantity must be positive");
        }
    }

    // Для десеріалізації
    Booking(std::string id, std::string productName, int quantity,
            std::string customerId, BookingStatus status,
            std::chrono::system_clock::time_point createdAt,
            std::chrono::system_clock::time_point expiresAt)
        : id_(std::move(id)),
          productName_(std::move(productName)),
          quantity_(quantity),
          customerId_(std::move(customerId)),
          status_(status),
          createdAt_(createdAt),
          expiresAt_(expiresAt) {}

    const std::string& getId() const { return id_; }
    const std::string& getProductName() const { return productName_; }
    int getQuantity() const { return quantity_; }
    const std::string& getCustomerId() const { return customerId_; }
    BookingStatus getStatus() const { return status_; }
    std::chrono::system_clock::time_point getCreatedAt() const { return createdAt_; }
    std::chrono::system_clock::time_point getExpiresAt() const { return expiresAt_; }

    void changeStatus(BookingStatus newStatus) {
        status_ = newStatus;
    }

    bool isExpired() const {
        return std::chrono::system_clock::now() > expiresAt_;
    }

    bool isActive() const {
        return status_ == BookingStatus::RESERVED || 
               status_ == BookingStatus::CONFIRMED;
    }
};

#endif
