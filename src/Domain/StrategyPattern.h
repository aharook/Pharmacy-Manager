#ifndef DOMAIN_STRATEGY_PATTERN_H
#define DOMAIN_STRATEGY_PATTERN_H

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>

class IDiscountStrategy {
public:
    virtual ~IDiscountStrategy() = default;
    virtual double calculate(double baseAmount) const = 0;
};

class PercentageDiscountStrategy final : public IDiscountStrategy {
private:
    double percent_;
public:
    explicit PercentageDiscountStrategy(double percent)
        : percent_(percent) {
        if (percent_ < 0.0 || percent_ > 1.0) {
            throw std::invalid_argument("percent must be between 0 and 1");
        }
    }

    double calculate(double baseAmount) const override {
        if (baseAmount <= 0.0) {
            return 0.0;
        }
        double discount = baseAmount * percent_;
        return std::max(0.0, baseAmount - discount);
    }
};

class FixedDiscountStrategy final : public IDiscountStrategy {
private:
    double amount_;
public:
    explicit FixedDiscountStrategy(double amount)
        : amount_(amount) {
        if (amount_ < 0.0) {
            throw std::invalid_argument("amount must be non-negative");
        }
    }

    double calculate(double baseAmount) const override {
        if (baseAmount <= 0.0) {
            return 0.0;
        }
        return std::max(0.0, baseAmount - amount_);
    }

};

class DiscountCard {
private:

    std::string cardType_;
    std::shared_ptr<IDiscountStrategy> strategy_;

public:

    DiscountCard(std::string cardType, std::shared_ptr<IDiscountStrategy> strategy)
        : cardType_(std::move(cardType)), strategy_(std::move(strategy)) {}

    const std::string& getCardType() const {
        return cardType_;
    }

    double applyDiscount(double baseAmount) const {
        if (!strategy_) {
            return baseAmount;
        }
        return strategy_->calculate(baseAmount);
    }

};



#endif
