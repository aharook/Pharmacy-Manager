#ifndef APPLICATION_REPORT_SERVICE_H
#define APPLICATION_REPORT_SERVICE_H

#include <string>
#include <vector>
#include <unordered_map>

#include "../Domain/OrderRepository.h"
#include "../Domain/BookingRepository.h"
#include "../Domain/RepositoryPattern.h"

struct ProductSalesStats {
    std::string productName;
    int totalQuantitySold;
    double totalRevenue;
    int orderCount;
};

struct OrderStats {
    int totalOrders;
    int pendingOrders;
    int deliveredOrders;
    double totalRevenue;
    double averageOrderValue;
};

class ReportService {
private:
    IOrderRepository& orderRepository_;
    IBookingRepository& bookingRepository_;
    IInventoryRepository& inventoryRepository_;

public:
    ReportService(IOrderRepository& orderRepository,
                  IBookingRepository& bookingRepository,
                  IInventoryRepository& inventoryRepository)
        : orderRepository_(orderRepository),
          bookingRepository_(bookingRepository),
          inventoryRepository_(inventoryRepository) {}

    // Get overall order statistics
    OrderStats getOrderStatistics() const;

    // Get sales statistics by product
    std::vector<ProductSalesStats> getProductSalesStats() const;

    // Find products with low stock (less than threshold)
    std::vector<std::string> getLowStockProducts(int threshold) const;

    // Get top selling products
    std::vector<ProductSalesStats> getTopSellingProducts(int limit = 10) const;

    // Get active booking count
    int getActiveBookingCount() const {
        return bookingRepository_.getActive().size();
    }

    // Get total inventory value
    double getTotalInventoryValue() const;

    // Search products by name (partial match)
    std::vector<std::string> searchProductsByName(const std::string& query) const;

    // Filter products with stock above minimum
    std::vector<std::string> getProductsInStock(int minQuantity = 1) const;

    // Sort products by price
    std::vector<std::string> getProductsSortedByPrice(bool ascending = true) const;
};

#endif
