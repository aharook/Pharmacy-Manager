#ifndef APPLICATION_ORDER_SERVICE_H
#define APPLICATION_ORDER_SERVICE_H

#include <memory>
#include <string>
#include <vector>
#include "../Domain/Order.h"
#include "../Domain/SaleFactory.h"
#include "../Infrastructure/FileOrderRepository.h"
#include "../Infrastructure/FileProductRepository.h"

struct OrderResult {
    std::string orderId;
    double total;
};

class OrderService {
private:
    FileOrderRepository& orderRepository_;
    std::unique_ptr<FileProductRepository> productRepository_;

public:
    OrderService(FileOrderRepository& orderRepository, 
                 const std::string& productsFilePath = "pharmacy_products.txt");

    OrderResult createOrder(const std::string& productName, int quantity, int saleType);

    std::vector<Order> getAllOrders() const;
};

#endif
