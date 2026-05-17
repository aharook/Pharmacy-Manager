#include "OrderService.h"
#include <stdexcept>
#include <algorithm>

OrderService::OrderService(IOrderRepository& orderRepository, IProductRepository& productRepository)
    : orderRepository_(orderRepository), productRepository_(productRepository) {
}

OrderResult OrderService::createOrder(const std::string& productName, int quantity, int saleType) {
    if (productName.empty()) {
        throw std::invalid_argument("Product name cannot be empty!");
    }

    if (quantity <= 0) {
        throw std::invalid_argument("Quantity must be positive!");
    }

    if (saleType != 1 && saleType != 2) {
        throw std::invalid_argument("Invalid sale type (use 1 for Direct, 2 for Booking)!");
    }

    Product* selectedProduct = productRepository_.findByName(productName);
    if (!selectedProduct) {
        throw std::runtime_error("Product not found!");
    }

    if (selectedProduct->getPackCount() < quantity) {
        throw std::runtime_error("Insufficient stock! Available: " + std::to_string(selectedProduct->getPackCount()));
    }

    SaleType saletype_enum = (saleType == 2) ? SaleType::BOOKING : SaleType::DIRECT;

    auto allOrders = orderRepository_.getAll();
    int nextId = 1;
    for (const auto& order : allOrders) {
        std::string orderId = order.getId();
        if (orderId.substr(0, 3) == "ORD") {
            try {
                int id = std::stoi(orderId.substr(3));
                nextId = std::max(nextId, id + 1);
            } catch (...) {
            }
        }
    }
    std::string orderId = "ORD" + std::to_string(nextId);

    Order order(orderId, saletype_enum);
    order.addItem(OrderItem(*selectedProduct, quantity));

    Sale* sale = SaleFactory::createSale(saletype_enum);
    order.setTotal(sale->calculate(order.getItems()));
    delete sale;

    orderRepository_.save(order);

    selectedProduct->decreasePackCount(quantity);
    productRepository_.save(*selectedProduct);

    OrderResult result;
    result.orderId = orderId;
    result.total = order.getTotal();
    return result;
}

std::vector<Order> OrderService::getAllOrders() const {
    return orderRepository_.getAll();
}

void OrderService::markOrderAsReceivedAndDelete(const std::string& orderId) {
    try {
        const Order& order = orderRepository_.getById(orderId);
        Order receivedOrder = order;
        receivedOrder.markAsReceived();
        orderRepository_.deleteById(orderId);
    } catch (const std::runtime_error& e) {
        throw std::runtime_error("Cannot mark order as received: " + std::string(e.what()));
    }
}
