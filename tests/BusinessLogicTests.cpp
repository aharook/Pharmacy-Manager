#include <memory>
#include <stdexcept>
#include <vector>

#include <gtest/gtest.h>

#include "Application/InventoryService.h"
#include "Application/OrderDtos.h"
#include "Application/OrderService.h"
#include "Application/PharmacyController.h"
#include "Domain/Product.h"
#include "Domain/RepositoryPattern.h"
#include "Domain/StrategyPattern.h"

TEST(InventoryServiceTests, ReserveItemsReturnsFalseForInvalidQuantity) {
    InMemoryInventoryRepository repo;
    repo.save(Product("Aspirin", 10, 5.0));

    InventoryService inventory(repo);
    EXPECT_FALSE(inventory.reserveItems("Aspirin", 0));
    EXPECT_FALSE(inventory.reserveItems("Aspirin", -1));
}

TEST(InventoryServiceTests, ReserveItemsChecksStock) {
    InMemoryInventoryRepository repo;
    repo.save(Product("Aspirin", 2, 5.0));

    InventoryService inventory(repo);
    EXPECT_TRUE(inventory.reserveItems("Aspirin", 2));
    EXPECT_FALSE(inventory.reserveItems("Aspirin", 3));
    EXPECT_FALSE(inventory.reserveItems("Missing", 1));
}

TEST(InventoryServiceTests, DecreaseStockUpdatesRepository) {
    InMemoryInventoryRepository repo;
    repo.save(Product("Aspirin", 5, 5.0));

    InventoryService inventory(repo);
    EXPECT_TRUE(inventory.decreaseStock("Aspirin", 3));

    auto updated = repo.getByName("Aspirin");
    ASSERT_TRUE(updated.has_value());
    EXPECT_EQ(updated->getPackCount(), 2);
}

TEST(OrderServiceTests, CalculatesTotalWithDiscount) {
    OrderService orderService;

    std::vector<OrderItem> items;
    items.emplace_back(Product("Aspirin", 10, 5.0), 2);
    items.emplace_back(Product("VitaminC", 10, 3.0), 1);

    auto order = orderService.createOrder(items);
    auto strategy = std::make_shared<PercentageDiscountStrategy>(0.1);
    DiscountCard card("Silver", strategy);

    double total = orderService.calculateTotal(order, &card);
    EXPECT_DOUBLE_EQ(total, 11.7);
}

TEST(PharmacyControllerTests, SellProductsDecreasesStockAndReturnsTotal) {
    InMemoryInventoryRepository repo;
    repo.save(Product("Aspirin", 10, 5.0));
    repo.save(Product("VitaminC", 5, 3.0));

    InventoryService inventory(repo);
    OrderService orderService;
    PharmacyController controller(inventory, orderService);

    std::vector<OrderRequestItem> request = {
        {"Aspirin", 2},
        {"VitaminC", 1}
    };

    auto strategy = std::make_shared<FixedDiscountStrategy>(1.0);
    DiscountCard card("Bronze", strategy);

    double total = controller.sellProducts(request, &card);
    EXPECT_DOUBLE_EQ(total, 12.0);

    auto aspirin = repo.getByName("Aspirin");
    auto vitamin = repo.getByName("VitaminC");
    ASSERT_TRUE(aspirin.has_value());
    ASSERT_TRUE(vitamin.has_value());
    EXPECT_EQ(aspirin->getPackCount(), 8);
    EXPECT_EQ(vitamin->getPackCount(), 4);
}

TEST(PharmacyControllerTests, CreateOrderThrowsWhenStockInsufficient) {
    InMemoryInventoryRepository repo;
    repo.save(Product("Aspirin", 1, 5.0));

    InventoryService inventory(repo);
    OrderService orderService;
    PharmacyController controller(inventory, orderService);

    std::vector<OrderRequestItem> request = {
        {"Aspirin", 2}
    };

    EXPECT_THROW(controller.createOrder(request), std::runtime_error);
}
