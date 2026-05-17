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
    return "test_error_" + name + ".txt";
}

void removeFile(const std::string& path) {
    std::error_code ec;
    std::filesystem::remove(path, ec);
}
}

TEST(InventoryServiceErrorTests, AddProductEmptyName) {
    const std::string productsFile = tempFilePath("products_empty_name");
    removeFile(productsFile);

    {
        FileProductRepository repo(productsFile);
        InventoryService service(repo);
        EXPECT_THROW(service.addProduct("", 10, 1.0), std::invalid_argument);
    }

    removeFile(productsFile);
}

TEST(InventoryServiceErrorTests, AddDuplicateProduct) {
    const std::string productsFile = tempFilePath("products_duplicate");
    removeFile(productsFile);

    {
        FileProductRepository repo(productsFile);
        InventoryService service(repo);
        service.addProduct("Aspirin", 20, 1.50);

        EXPECT_THROW(service.addProduct("Aspirin", 10, 2.00), std::invalid_argument);
    }

    removeFile(productsFile);
}

TEST(OrderServiceErrorTests, CreateOrderInvalidSaleType) {
    const std::string productsFile = tempFilePath("products_invalid_sale_type");
    const std::string ordersFile = tempFilePath("orders_invalid_sale_type");
    removeFile(productsFile);
    removeFile(ordersFile);

    {
        FileProductRepository productRepo(productsFile);
        InventoryService inventory(productRepo);
        inventory.addProduct("Aspirin", 10, 5.0);

        FileOrderRepository orderRepo(ordersFile);
        OrderService service(orderRepo, productRepo);

        EXPECT_THROW(service.createOrder("Aspirin", 1, 5), std::invalid_argument);
    }

    removeFile(productsFile);
    removeFile(ordersFile);
}

TEST(OrderServiceErrorTests, CreateOrderInsufficientStock) {
    const std::string productsFile = tempFilePath("products_low_stock");
    const std::string ordersFile = tempFilePath("orders_low_stock");
    removeFile(productsFile);
    removeFile(ordersFile);

    {
        FileProductRepository productRepo(productsFile);
        InventoryService inventory(productRepo);
        inventory.addProduct("Aspirin", 1, 5.0);

        FileOrderRepository orderRepo(ordersFile);
        OrderService service(orderRepo, productRepo);

        EXPECT_THROW(service.createOrder("Aspirin", 2, 1), std::runtime_error);
    }

    removeFile(productsFile);
    removeFile(ordersFile);
}

TEST(BookingServiceErrorTests, CreateBookingEmptyOrderId) {
    const std::string ordersFile = tempFilePath("orders_empty_booking");
    removeFile(ordersFile);

    {
        FileOrderRepository orderRepo(ordersFile);
        BookingService service(orderRepo);

        EXPECT_THROW(service.createBooking(""), std::invalid_argument);
    }

    removeFile(ordersFile);
}

TEST(BookingServiceErrorTests, MarkBookingNotFound) {
    const std::string ordersFile = tempFilePath("orders_booking_not_found");
    removeFile(ordersFile);

    {
        FileOrderRepository orderRepo(ordersFile);
        BookingService service(orderRepo);

        EXPECT_THROW(service.markBookingAsMissed("ORD999"), std::runtime_error);
    }

    removeFile(ordersFile);
}

