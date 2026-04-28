#ifndef CONSOLE_PHARMACY_CONSOLE_H
#define CONSOLE_PHARMACY_CONSOLE_H

#include <memory>
#include <string>
#include <vector>

#include "../Application/BookingService.h"
#include "../Application/DiscountService.h"
#include "../Application/InventoryService.h"
#include "../Application/OrderService.h"
#include "../Application/PharmacyController.h"
#include "../Application/ReportService.h"
#include "../Domain/BookingRepository.h"
#include "../Domain/OrderRepository.h"
#include "../Domain/RepositoryPattern.h"

class PharmacyConsole {
private:
    InventoryService& inventoryService_;
    OrderService& orderService_;
    PharmacyController& controller_;
    BookingService& bookingService_;
    DiscountService& discountService_;
    ReportService& reportService_;
    IOrderRepository& orderRepository_;

public:
    PharmacyConsole(InventoryService& inventoryService,
                    OrderService& orderService,
                    PharmacyController& controller,
                    BookingService& bookingService,
                    DiscountService& discountService,
                    ReportService& reportService,
                    IOrderRepository& orderRepository)
        : inventoryService_(inventoryService),
          orderService_(orderService),
          controller_(controller),
          bookingService_(bookingService),
          discountService_(discountService),
          reportService_(reportService),
          orderRepository_(orderRepository) {}

    void run();

private:
    void showMainMenu();
    
    // Inventory operations
    void showInventoryMenu();
    void searchProduct();
    void viewAllProducts();
    void addProduct();
    
    // Order operations
    void showOrderMenu();
    void createOrder();
    void viewOrders();
    void changeOrderStatus();
    void viewOrderDetails();
    
    // Booking operations
    void showBookingMenu();
    void createBooking();
    void confirmBooking();
    void cancelBooking();
    void viewActiveBookings();
    void viewCustomerBookings();
    
    // Discount operations
    void showDiscountMenu();
    void listAvailableDiscounts();
    void testDiscount();
    
    // Reporting & Analytics
    void showReportMenu();
    void viewOrderStatistics();
    void viewProductSalesStats();
    void viewLowStockProducts();
    void viewTopSellingProducts();
    void searchProductsByName();
    void viewProductsInStock();
    
    // Persistence
    void showPersistenceMenu();
    void saveData();
    void loadData();
    
    // Utility
    std::string getUserInput(const std::string& prompt);
    int getUserChoice();
    void displaySuccess(const std::string& message);
    void displayError(const std::string& message);
};

#endif
