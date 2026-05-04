#ifndef CONSOLE_PHARMACY_CONSOLE_H
#define CONSOLE_PHARMACY_CONSOLE_H

#include <string>
#include <memory>

#include "InventoryManager.h"
#include "OrderManager.h"
#include "BookingManager.h"
#include "../Infrastructure/FileOrderRepository.h"

class PharmacyConsole {
private:
    std::unique_ptr<InventoryManager> inventoryManager_;
    std::unique_ptr<OrderManager> orderManager_;
    std::unique_ptr<BookingManager> bookingManager_;
    
    int getUserChoice();
    void displayMessage(const std::string& msg);
    void displayError(const std::string& error);

public:
    explicit PharmacyConsole(FileOrderRepository& orderRepository, const std::string& productsFilePath = "pharmacy_products.txt");

    void run();


};

#endif
