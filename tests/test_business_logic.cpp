#include <gtest/gtest.h>
#include <memory>

#include "Calculations/BookingCalculation.h"
#include "Domain/Order.h"
#include "Domain/OrderItem.h"
#include "Domain/Product.h"
#include "Calculations/SaleFactory.h"

TEST(ProductLogicTests, CreateProductWithValidData) {
    Product product("Aspirin", 10, 5.0);

    EXPECT_EQ(product.getName(), "Aspirin");
    EXPECT_EQ(product.getPackCount(), 10);
    EXPECT_DOUBLE_EQ(product.getPrice(), 5.0);
}

TEST(OrderItemLogicTests, CalculateLineTotal) {
    Product product("Medicine A", 10, 10.0);
    OrderItem item(product, 3);

    EXPECT_EQ(item.getQuantity(), 3);
    EXPECT_DOUBLE_EQ(item.getUnitPrice(), 10.0);
    EXPECT_DOUBLE_EQ(item.getLineTotal(), 30.0);
}

TEST(SaleFactoryLogicTests, DirectSaleCalculation) {
    Product product1("Drug A", 10, 100.0);
    Product product2("Drug B", 5, 50.0);

    std::vector<OrderItem> items;
    items.emplace_back(product1, 2);
    items.emplace_back(product2, 1);

    std::unique_ptr<Sale> sale(SaleFactory::createSale(SaleType::DIRECT));
    double total = sale->calculate(items);

    EXPECT_DOUBLE_EQ(total, 250.0);
}

TEST(SaleFactoryLogicTests, BookingSaleCalculation) {
    Product product("Medicine", 10, 100.0);

    std::vector<OrderItem> items;
    items.emplace_back(product, 2);

    std::unique_ptr<Sale> sale(SaleFactory::createSale(SaleType::BOOKING));
    double total = sale->calculate(items);

    EXPECT_DOUBLE_EQ(total, 230.0);
}

TEST(OrderLogicTests, CreateOrderAndCalculateTotal) {
    Product product1("Paracetamol", 15, 2.0);
    Product product2("Ibuprofen", 10, 3.0);

    Order order("ORD001", SaleType::DIRECT);
    order.addItem(OrderItem(product1, 5));
    order.addItem(OrderItem(product2, 2));

    std::unique_ptr<Sale> sale(SaleFactory::createSale(SaleType::DIRECT));
    order.setTotal(sale->calculate(order.getItems()));

    EXPECT_DOUBLE_EQ(order.getTotal(), 16.0);
}

TEST(BookingLogicTests, MissedBookingPenalty) {
    double originalTotal = 100.0;

    EXPECT_DOUBLE_EQ(BookingCalculation::calculatePenalty(originalTotal, false), 100.0);

    EXPECT_DOUBLE_EQ(BookingCalculation::calculatePenalty(originalTotal, true), 80.0);
}
