#include "ReportService.h"

#include <algorithm>

OrderStats ReportService::getOrderStatistics() const {
    auto allOrders = orderRepository_.getAll();
    
    OrderStats stats{
        static_cast<int>(allOrders.size()),
        0,
        0,
        0.0,
        0.0
    };

    for (const auto& order : allOrders) {
        stats.totalRevenue += order.getTotal();
        
        if (order.getStatus() == OrderStatus::PENDING) {
            stats.pendingOrders++;
        } else if (order.getStatus() == OrderStatus::DELIVERED) {
            stats.deliveredOrders++;
        }
    }

    if (stats.totalOrders > 0) {
        stats.averageOrderValue = stats.totalRevenue / stats.totalOrders;
    }

    return stats;
}

std::vector<ProductSalesStats> ReportService::getProductSalesStats() const {
    std::unordered_map<std::string, ProductSalesStats> statsMap;
    
    auto allOrders = orderRepository_.getAll();
    
    for (const auto& order : allOrders) {
        for (const auto& item : order.getItems()) {
            const auto& productName = item.getProduct().getName();
            
            if (statsMap.find(productName) == statsMap.end()) {
                statsMap[productName] = {productName, 0, 0.0, 0};
            }
            
            statsMap[productName].totalQuantitySold += item.getQuantity();
            statsMap[productName].totalRevenue += item.getLineTotal();
            statsMap[productName].orderCount++;
        }
    }

    std::vector<ProductSalesStats> result;
    for (const auto& [_, stats] : statsMap) {
        result.push_back(stats);
    }

    return result;
}

std::vector<std::string> ReportService::getLowStockProducts(int threshold) const {
    std::vector<std::string> result;
    
    // This requires iterating through inventory
    // In a real implementation, we'd need a method to get all products
    // For now, we check through orders to find product names
    auto allOrders = orderRepository_.getAll();
    std::unordered_map<std::string, int> productStocks;
    
    for (const auto& order : allOrders) {
        for (const auto& item : order.getItems()) {
            auto product = inventoryRepository_.getByName(item.getProduct().getName());
            if (product && product->getPackCount() < threshold) {
                result.push_back(product->getName());
            }
        }
    }

    // Remove duplicates
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

std::vector<ProductSalesStats> ReportService::getTopSellingProducts(int limit) const {
    auto stats = getProductSalesStats();
    
    // Sort by total quantity sold (descending)
    std::sort(stats.begin(), stats.end(),
        [](const ProductSalesStats& a, const ProductSalesStats& b) {
            return a.totalQuantitySold > b.totalQuantitySold;
        });

    if (stats.size() > static_cast<size_t>(limit)) {
        stats.resize(limit);
    }

    return stats;
}

double ReportService::getTotalInventoryValue() const {
    double totalValue = 0.0;
    
    // We need to iterate through known products
    // This is a limitation of the current repository design
    // In production, we'd have a getAll() method in IInventoryRepository
    
    return totalValue;
}

std::vector<std::string> ReportService::searchProductsByName(
    const std::string& query) const {
    std::vector<std::string> result;
    
    // Search through orders to find product names
    auto allOrders = orderRepository_.getAll();
    std::unordered_map<std::string, bool> found;
    
    for (const auto& order : allOrders) {
        for (const auto& item : order.getItems()) {
            const auto& productName = item.getProduct().getName();
            if (found.find(productName) == found.end()) {
                found[productName] = true;
                
                // Simple substring search
                if (productName.find(query) != std::string::npos) {
                    result.push_back(productName);
                }
            }
        }
    }

    return result;
}

std::vector<std::string> ReportService::getProductsInStock(int minQuantity) const {
    std::vector<std::string> result;
    
    // Search through orders and check current stock
    auto allOrders = orderRepository_.getAll();
    std::unordered_map<std::string, bool> checked;
    
    for (const auto& order : allOrders) {
        for (const auto& item : order.getItems()) {
            const auto& productName = item.getProduct().getName();
            if (checked.find(productName) == checked.end()) {
                checked[productName] = true;
                
                auto product = inventoryRepository_.getByName(productName);
                if (product && product->getPackCount() >= minQuantity) {
                    result.push_back(productName);
                }
            }
        }
    }

    return result;
}

std::vector<std::string> ReportService::getProductsSortedByPrice(bool ascending) const {
    std::vector<std::pair<std::string, double>> products;
    
    // Collect products from orders
    auto allOrders = orderRepository_.getAll();
    std::unordered_map<std::string, double> productPrices;
    
    for (const auto& order : allOrders) {
        for (const auto& item : order.getItems()) {
            productPrices[item.getProduct().getName()] = item.getProduct().getPrice();
        }
    }

    // Sort by price
    for (const auto& [name, price] : productPrices) {
        products.push_back({name, price});
    }

    std::sort(products.begin(), products.end(),
        [ascending](const auto& a, const auto& b) {
            return ascending ? a.second < b.second : a.second > b.second;
        });

    std::vector<std::string> result;
    for (const auto& [name, _] : products) {
        result.push_back(name);
    }

    return result;
}
