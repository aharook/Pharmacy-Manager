#ifndef APPLICATION_DISCOUNT_SERVICE_H
#define APPLICATION_DISCOUNT_SERVICE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Domain/StrategyPattern.h"

class DiscountService {
private:
    std::unordered_map<std::string, std::shared_ptr<IDiscountStrategy>> discountStrategies_;

    // Business Rule: Maximum discount percentage
    static constexpr double MAX_DISCOUNT_PERCENT = 0.5;  // 50%

    // Business Rule: Maximum fixed discount amount
    static constexpr double MAX_FIXED_DISCOUNT = 100.0;  // 100 currency units

    // Business Rule: Minimum purchase amount for discount eligibility
    static constexpr double MIN_PURCHASE_AMOUNT = 10.0;

public:
    DiscountService() {
        registerDefaultStrategies();
    }

    // Register discount strategy for card type
    bool registerStrategy(const std::string& cardType, 
                         std::shared_ptr<IDiscountStrategy> strategy);

    // Get discount card by type
    std::optional<DiscountCard> getDiscountCard(const std::string& cardType) const;

    // Calculate final price after discount
    double applyDiscount(const std::string& cardType, double baseAmount) const;

    // Check if amount qualifies for discount
    bool isEligibleForDiscount(double amount) const {
        return amount >= MIN_PURCHASE_AMOUNT;
    }

    // Get list of available discount cards
    std::vector<std::string> getAvailableCards() const;

    // Business Rule: Apply combination of discounts (stacked discounts)
    // For example: 10% loyalty + 5% seasonal
    double applyCombinedDiscount(const std::vector<std::string>& cardTypes, 
                                 double baseAmount) const;

private:
    void registerDefaultStrategies();
};

#endif
