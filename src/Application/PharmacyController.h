#ifndef APPLICATION_PHARMACY_CONTROLLER_H
#define APPLICATION_PHARMACY_CONTROLLER_H

#include <stdexcept>
#include <vector>

#include "InventoryService.h"
#include "OrderDtos.h"
#include "OrderService.h"
#include "../Domain/Order.h"
#include "../Domain/StrategyPattern.h"

class PharmacyController {
private:
    InventoryService& inventoryService_;
    OrderService& orderService_;
public:
    PharmacyController(InventoryService& inventoryService, OrderService& orderService)
        : inventoryService_(inventoryService), orderService_(orderService) {}

    Order createOrder(const std::vector<OrderRequestItem>& items) const {
        std::vector<OrderItem> orderItems;
        orderItems.reserve(items.size());

        for (const auto& request : items) {
            if (!inventoryService_.reserveItems(request.productName, request.quantity)) {
                throw std::runtime_error("not enough stock for: " + request.productName);
            }
            auto product = inventoryService_.findProduct(request.productName);
            if (!product) {
                throw std::runtime_error("product not found: " + request.productName);
            }
            orderItems.emplace_back(*product, request.quantity);
        }

        return orderService_.createOrder(orderItems);
    }

    double sellProducts(const std::vector<OrderRequestItem>& items,
                        const DiscountCard* discountCard) {
        auto order = createOrder(items);
        double total = orderService_.calculateTotal(order, discountCard);

        for (const auto& request : items) {
            if (!inventoryService_.decreaseStock(request.productName, request.quantity)) {
                throw std::runtime_error("failed to decrease stock for: " + request.productName);
            }
        }

        return total;
    }
};

#endif
