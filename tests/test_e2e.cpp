#include <gtest/gtest.h>
#include <filesystem>
#include <string>

#include "Application/BookingService.h"
#include "Application/InventoryService.h"
#include "Application/OrderService.h"
#include "Infrastructure/FileOrderRepository.h"
#include "Infrastructure/FileProductRepository.h"

namespace {
std::string tempFilePath(const std::string& name) {
    return "test_e2e_" + name + ".txt";
}

void removeFile(const std::string& path) {
    std::filesystem::remove(path);
}
}

TEST(EndToEndTests, CreateOrderAndBookingFlow) {
    const std::string productsFile = tempFilePath("products_flow");
    const std::string ordersFile = tempFilePath("orders_flow");
    removeFile(productsFile);
    removeFile(ordersFile);

    {
        FileProductRepository productRepo(productsFile);
        InventoryService inventory(productRepo);
        inventory.addProduct("Test Drug", 10, 5.0);

        FileOrderRepository orderRepo(ordersFile);
        OrderService orderService(orderRepo, productRepo);

        OrderResult orderResult = orderService.createOrder("Test Drug", 2, 2);
        EXPECT_FALSE(orderResult.orderId.empty());
        EXPECT_DOUBLE_EQ(orderResult.total, 11.50);

        BookingService bookingService(orderRepo);
        EXPECT_NO_THROW(bookingService.createBooking(orderResult.orderId));

        BookingResult bookingResult = bookingService.markBookingAsMissed(orderResult.orderId);
        EXPECT_DOUBLE_EQ(bookingResult.penaltyAmount, 9.20);

        FileProductRepository productRepoReload(productsFile);
        InventoryService inventoryReload(productRepoReload);
        const auto& products = inventoryReload.getAllProducts();
        ASSERT_EQ(products.size(), 1u);
        EXPECT_EQ(products[0].getPackCount(), 8);
    }

    removeFile(productsFile);
    removeFile(ordersFile);
}
