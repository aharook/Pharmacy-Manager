#include "OrderService.h"

OrderService::OrderService(FileOrderRepository& orderRepository, 
                           const std::string& productsFilePath)
    : orderRepository_(orderRepository) {
    productRepository_ = std::make_unique<FileProductRepository>(productsFilePath);
}

OrderResult OrderService::createOrder(const std::string& productName, int quantity, int saleType) {
    OrderResult result;
    result.success = false;
    result.orderId = "";
    result.total = 0.0;

    // Validate inputs
    if (productName.empty()) {
        result.message = "Product name cannot be empty!";
        return result;
    }

    if (quantity <= 0) {
        result.message = "Quantity must be positive!";
        return result;
    }

    if (saleType != 1 && saleType != 2) {
        result.message = "Invalid sale type (use 1 for Direct, 2 for Booking)!";
        return result;
    }

    // Find product
    Product* selectedProduct = productRepository_->findByName(productName);
    if (!selectedProduct) {
        result.message = "Product not found!";
        return result;
    }

    // Check stock
    if (selectedProduct->getPackCount() < quantity) {
        result.message = "Insufficient stock! Available: " + std::to_string(selectedProduct->getPackCount());
        return result;
    }

    try {
        // Create order
        SaleType saletype_enum = (saleType == 2) ? SaleType::BOOKING : SaleType::DIRECT;
        std::string orderId = "ORD" + std::to_string(orderRepository_.getAll().size() + 1);
        
        Order order(orderId, saletype_enum);
        order.addItem(OrderItem(*selectedProduct, quantity));
        order.calculateTotal();

        // Save order
        orderRepository_.save(order);

        // Update product stock
        selectedProduct->decreasePackCount(quantity);
        productRepository_->save(*selectedProduct);

        result.success = true;
        result.orderId = orderId;
        result.total = order.getTotal();
        result.message = "Order created successfully";
        return result;
    } catch (const std::exception& e) {
        result.message = std::string("Error: ") + e.what();
        return result;
    }
}

std::vector<Order> OrderService::getAllOrders() const {
    return orderRepository_.getAll();
}
