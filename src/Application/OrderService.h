#ifndef APPLICATION_ORDER_SERVICE_H
#define APPLICATION_ORDER_SERVICE_H

#include <memory>
#include <string>
#include <vector>
#include "../Domain/Order.h"
#include "../Domain/OrderRepository.h"
#include "../Domain/ProductRepository.h"
#include "../Calculations/SaleFactory.h"

struct OrderResult {
    std::string orderId;
    double total;
};

class OrderService {
private:
    IOrderRepository& orderRepository_;
    IProductRepository& productRepository_;

public:
    OrderService(IOrderRepository& orderRepository, IProductRepository& productRepository);

    OrderResult createOrder(const std::string& productName, int quantity, int saleType);

    std::vector<Order> getAllOrders() const;

    void markOrderAsReceivedAndDelete(const std::string& orderId);
};

#endif
