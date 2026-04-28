#include "DiscountService.h"

bool DiscountService::registerStrategy(
    const std::string& cardType,
    std::shared_ptr<IDiscountStrategy> strategy) {
    
    if (!strategy || cardType.empty()) {
        return false;
    }

    discountStrategies_[cardType] = strategy;
    return true;
}

std::optional<DiscountCard> DiscountService::getDiscountCard(
    const std::string& cardType) const {
    
    auto it = discountStrategies_.find(cardType);
    if (it == discountStrategies_.end()) {
        return std::nullopt;
    }

    return DiscountCard(cardType, it->second);
}

double DiscountService::applyDiscount(
    const std::string& cardType,
    double baseAmount) const {
    
    auto card = getDiscountCard(cardType);
    if (!card) {
        return baseAmount;
    }

    // Business Rule: Check eligibility
    if (!isEligibleForDiscount(baseAmount)) {
        return baseAmount;
    }

    return card->applyDiscount(baseAmount);
}

std::vector<std::string> DiscountService::getAvailableCards() const {
    std::vector<std::string> cards;
    for (const auto& [cardType, _] : discountStrategies_) {
        cards.push_back(cardType);
    }
    return cards;
}

double DiscountService::applyCombinedDiscount(
    const std::vector<std::string>& cardTypes,
    double baseAmount) const {
    
    // Business Rule: Apply discounts in sequence
    double currentAmount = baseAmount;
    
    for (const auto& cardType : cardTypes) {
        auto card = getDiscountCard(cardType);
        if (card && isEligibleForDiscount(currentAmount)) {
            currentAmount = card->applyDiscount(currentAmount);
        }
    }

    return currentAmount;
}

void DiscountService::registerDefaultStrategies() {
    // Register default discount strategies
    auto silverStrategy = std::make_shared<PercentageDiscountStrategy>(0.10);  // 10%
    auto goldStrategy = std::make_shared<PercentageDiscountStrategy>(0.20);    // 20%
    auto platinumStrategy = std::make_shared<PercentageDiscountStrategy>(0.30); // 30%
    auto bronzeStrategy = std::make_shared<FixedDiscountStrategy>(5.0);        // 5 currency units
    
    registerStrategy("SILVER", silverStrategy);
    registerStrategy("GOLD", goldStrategy);
    registerStrategy("PLATINUM", platinumStrategy);
    registerStrategy("BRONZE", bronzeStrategy);
}
