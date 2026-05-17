#ifndef CALCULATIONS_SALE_FACTORY_H
#define CALCULATIONS_SALE_FACTORY_H

#include <stdexcept>
#include <string>
#include <vector>
#include "../Domain/Order.h"
#include "../Domain/OrderItem.h"

class Sale {
public:
    virtual ~Sale() = default;
    virtual double calculate(const std::vector<OrderItem>& items) = 0;
    virtual SaleType getType() const = 0;
};

class DirectSale : public Sale {
public:
    double calculate(const std::vector<OrderItem>& items) override {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getProduct().getPrice() * item.getQuantity();
        }
        return total;
    }

    SaleType getType() const override {
        return SaleType::DIRECT;
    }
};

class BookingSale : public Sale {
private:
    static constexpr double SURCHARGE = 1.15;
public:
    double calculate(const std::vector<OrderItem>& items) override {
        double total = 0.0;
        for (const auto& item : items) {
            total += item.getProduct().getPrice() * item.getQuantity();
        }
        return total * SURCHARGE;
    }

    SaleType getType() const override {
        return SaleType::BOOKING;
    }
};

class SaleFactory {
public:
    static Sale* createSale(SaleType type) {
        switch (type) {
            case SaleType::DIRECT:
                return new DirectSale();
            case SaleType::BOOKING:
                return new BookingSale();
            default:
                throw std::invalid_argument("Unknown sale type");
        }
    }
};

#endif
