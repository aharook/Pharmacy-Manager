#ifndef CONSOLE_ORDER_MANAGER_H
#define CONSOLE_ORDER_MANAGER_H

#include <memory>
#include <string>
#include "OrderService.h"
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
    
    int getUserChoice();
    std::string getUserInput(const std::string& prompt);
    void displayMessage(const std::string& msg);
    void displayError(const std::string& error);
};

#endif
