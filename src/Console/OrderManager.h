#ifndef CONSOLE_ORDER_MANAGER_H
#define CONSOLE_ORDER_MANAGER_H

#include <memory>
#include <string>
#include "../Application/OrderService.h"
#include "../Infrastructure/FileOrderRepository.h"

class OrderManager {
private:
    std::unique_ptr<OrderService> orderService_;

public:
    OrderManager(FileOrderRepository& orderRepository, 
                 const std::string& productsFilePath = "pharmacy_products.txt");

    void run();

private:
    void createOrder();
    void viewOrders();
};

#endif
