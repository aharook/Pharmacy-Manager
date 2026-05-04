#include "OrderService.h"
#include <stdexcept>

OrderService::OrderService(FileOrderRepository& orderRepository, const std::string& productsFilePath)
    : orderRepository_(orderRepository) {
    productRepository_ = std::make_unique<FileProductRepository>(productsFilePath);
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

    Product* selectedProduct = productRepository_->findByName(productName);
    if (!selectedProduct) {
        throw std::runtime_error("Product not found!");
    }

    if (selectedProduct->getPackCount() < quantity) {
        throw std::runtime_error("Insufficient stock! Available: " + std::to_string(selectedProduct->getPackCount()));
    }

    SaleType saletype_enum = (saleType == 2) ? SaleType::BOOKING : SaleType::DIRECT;
    std::string orderId = "ORD" + std::to_string(orderRepository_.getAll().size() + 1);
    
    Order order(orderId, saletype_enum);
    order.addItem(OrderItem(*selectedProduct, quantity));
    order.calculateTotal();

    orderRepository_.save(order);

    selectedProduct->decreasePackCount(quantity);
    productRepository_->save(*selectedProduct);

    OrderResult result;
    result.orderId = orderId;
    result.total = order.getTotal();
    return result;
}

std::vector<Order> OrderService::getAllOrders() const {
    return orderRepository_.getAll();
}
