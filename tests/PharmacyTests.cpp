#include <gtest/gtest.h>
#include <memory>
#include <vector>

#include "Domain/Booking.h"
#include "Domain/OrderItem.h"
#include "Domain/Product.h"
#include "Domain/SaleFactory.h"
#include "Infrastructure/FileOrderRepository.h"

// TEST 1: Product creation and stock management
TEST(ProductTests, CreateProductWithValidData) {
    Product product("Aspirin", 10, 5.0);
    
    EXPECT_EQ(product.getName(), "Aspirin");
    EXPECT_EQ(product.getPackCount(), 10);
    EXPECT_EQ(product.getPrice(), 5.0);
}

// TEST 2: Stock decrease on sale
TEST(ProductTests, DecreaseStockOnSale) {
    Product product("Vitamin C", 20, 3.5);
    
    product.decreasePackCount(5);
    EXPECT_EQ(product.getPackCount(), 15);
    
    product.decreasePackCount(10);
    EXPECT_EQ(product.getPackCount(), 5);
}

// TEST 3: OrderItem calculation
TEST(OrderItemTests, CalculateLineTotal) {
    Product product("Medicine A", 10, 10.0);
    OrderItem item(product, 3);
    
    EXPECT_EQ(item.getQuantity(), 3);
    EXPECT_EQ(item.getUnitPrice(), 10.0);
    EXPECT_DOUBLE_EQ(item.getLineTotal(), 30.0);
}

// TEST 4: Direct sale (no surcharge)
TEST(SaleFactoryTests, DirectSaleCalculation) {
    Product product1("Drug A", 10, 100.0);
    Product product2("Drug B", 5, 50.0);
    
    std::vector<OrderItem> items;
    items.emplace_back(product1, 2);  // 200
    items.emplace_back(product2, 1);  // 50
    
    Sale* sale = SaleFactory::createSale(SaleType::DIRECT);
    double total = sale->calculate(items);
    
    EXPECT_DOUBLE_EQ(total, 250.0);  // 200 + 50, no surcharge
    delete sale;
}

// TEST 5: Booking sale (15% surcharge)
TEST(SaleFactoryTests, BookingSaleCalculation) {
    Product product("Medicine", 10, 100.0);
    
    std::vector<OrderItem> items;
    items.emplace_back(product, 2);  // 200
    
    Sale* sale = SaleFactory::createSale(SaleType::BOOKING);
    double total = sale->calculate(items);
    
    EXPECT_DOUBLE_EQ(total, 230.0);  // 200 * 1.15
    delete sale;
}

// TEST 6: Order with items and total calculation
TEST(OrderTests, CreateOrderAndCalculateTotal) {
    Product product1("Paracetamol", 15, 2.0);
    Product product2("Ibuprofen", 10, 3.0);
    
    Order order("ORD001", SaleType::DIRECT);
    order.addItem(OrderItem(product1, 5));    // 10
    order.addItem(OrderItem(product2, 2));    // 6
    
    order.calculateTotal();
    
    EXPECT_DOUBLE_EQ(order.getTotal(), 16.0);  // 10 + 6
}

// TEST 7: Booking penalty (20% deduction for missed booking)
TEST(BookingTests, MissedBookingPenalty) {
    Booking booking("ORD001", false);  // not missed
    double originalTotal = 100.0;
    
    EXPECT_DOUBLE_EQ(booking.calculatePenalty(originalTotal), 100.0);
    
    booking.markAsMissed();
    EXPECT_DOUBLE_EQ(booking.calculatePenalty(originalTotal), 80.0);  // 100 * 0.8
}

// TEST 8: OrderRepository save and retrieve
TEST(OrderRepositoryTests, SaveAndRetrieveOrder) {
    std::string testFile = "test_orders.txt";
    
    {
        FileOrderRepository repo(testFile);
        Order order("ORD001", SaleType::DIRECT);
        order.setTotal(150.0);
        repo.save(order);
    }
    
    {
        FileOrderRepository repo(testFile);
        const Order& loaded = repo.getById("ORD001");
        
        EXPECT_EQ(loaded.getId(), "ORD001");
        EXPECT_DOUBLE_EQ(loaded.getTotal(), 150.0);
        EXPECT_EQ(static_cast<int>(loaded.getSaleType()), 0);  // DIRECT
    }
}

// TEST 9: Multiple orders in repository
TEST(OrderRepositoryTests, SaveMultipleOrders) {
    std::string testFile = "test_orders_multiple.txt";
    
    {
        FileOrderRepository repo(testFile);
        
        Order order1("ORD001", SaleType::DIRECT);
        order1.setTotal(100.0);
        repo.save(order1);
        
        Order order2("ORD002", SaleType::BOOKING);
        order2.setTotal(115.0);
        repo.save(order2);
    }
    
    {
        FileOrderRepository repo(testFile);
        auto allOrders = repo.getAll();
        
        EXPECT_EQ(allOrders.size(), 2);
    }
}

// TEST 10: SaleFactory creates correct type
TEST(SaleFactoryTests, CreateDirectSale) {
    Sale* sale = SaleFactory::createSale(SaleType::DIRECT);
    EXPECT_EQ(sale->getType(), SaleType::DIRECT);
    delete sale;
}

// TEST 11: SaleFactory creates booking sale
TEST(SaleFactoryTests, CreateBookingSale) {
    Sale* sale = SaleFactory::createSale(SaleType::BOOKING);
    EXPECT_EQ(sale->getType(), SaleType::BOOKING);
    delete sale;
}

// TEST 12: OrderRepository getAll with empty repository
TEST(OrderRepositoryTests, GetAllEmptyRepository) {
    std::string testFile = "test_orders_empty.txt";
    FileOrderRepository repo(testFile);
    
    auto allOrders = repo.getAll();
    EXPECT_EQ(allOrders.size(), 0);
}
