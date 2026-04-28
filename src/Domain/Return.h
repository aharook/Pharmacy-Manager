#ifndef DOMAIN_RETURN_H
#define DOMAIN_RETURN_H

#include <chrono>
#include <string>
#include <utility>

class Return {
private:
    std::string id_;
    std::string orderId_;
    std::string productName_;
    int quantity_;
    bool isOverdue_;
    double penaltyAmount_;
    std::chrono::system_clock::time_point returnedAt_;

public:
    Return(std::string id, std::string orderId, std::string productName,
           int quantity, bool isOverdue, double penaltyAmount)
        : id_(std::move(id)),
          orderId_(std::move(orderId)),
          productName_(std::move(productName)),
          quantity_(quantity),
          isOverdue_(isOverdue),
          penaltyAmount_(penaltyAmount),
          returnedAt_(std::chrono::system_clock::now()) {
        if (quantity <= 0) {
            throw std::invalid_argument("quantity must be positive");
        }
        if (penaltyAmount < 0) {
            throw std::invalid_argument("penaltyAmount must be non-negative");
        }
    }

    // Для десеріалізації
    Return(std::string id, std::string orderId, std::string productName,
           int quantity, bool isOverdue, double penaltyAmount,
           std::chrono::system_clock::time_point returnedAt)
        : id_(std::move(id)),
          orderId_(std::move(orderId)),
          productName_(std::move(productName)),
          quantity_(quantity),
          isOverdue_(isOverdue),
          penaltyAmount_(penaltyAmount),
          returnedAt_(returnedAt) {}

    const std::string& getId() const { return id_; }
    const std::string& getOrderId() const { return orderId_; }
    const std::string& getProductName() const { return productName_; }
    int getQuantity() const { return quantity_; }
    bool getIsOverdue() const { return isOverdue_; }
    double getPenaltyAmount() const { return penaltyAmount_; }
    std::chrono::system_clock::time_point getReturnedAt() const { return returnedAt_; }
};

#endif
