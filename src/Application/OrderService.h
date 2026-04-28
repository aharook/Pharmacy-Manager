#ifndef APPLICATION_ORDER_SERVICE_H
#define APPLICATION_ORDER_SERVICE_H

#include <vector>

#include "../Domain/Order.h"
#include "../Domain/OrderItem.h"
#include "../Domain/StrategyPattern.h"

class OrderService {
public:
    Order createOrder(const std::vector<OrderItem>& items) const {
        Order order;
        for (const auto& item : items) {
            order.addItem(item);
        }
        order.calculateTotal();
        return order;
    }

    double calculateTotal(Order& order, const DiscountCard* discountCard) const {
        double baseTotal = order.calculateTotal();
        if (!discountCard) {
            return baseTotal;
        }
        return discountCard->applyDiscount(baseTotal);
    }
};

#endif
