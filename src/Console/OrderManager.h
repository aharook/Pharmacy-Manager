#ifndef CONSOLE_ORDER_MANAGER_H
#define CONSOLE_ORDER_MANAGER_H

#include <memory>
#include <string>
#include "../Application/OrderService.h"
#include "../Domain/OrderRepository.h"
#include "../Domain/ProductRepository.h"

class OrderManager {
private:
    std::unique_ptr<OrderService> orderService_;

public:
    OrderManager(IOrderRepository& orderRepository, IProductRepository& productRepository);

    void run();

private:
    void createOrder();
    void viewOrders();
    void markOrderAsReceived();

    int getUserChoice();
    std::string getUserInput(const std::string& prompt);
    void displayMessage(const std::string& msg);
    void displayError(const std::string& error);
};

#endif
