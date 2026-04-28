#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <chrono>

#include "Application/BookingService.h"
#include "Application/DiscountService.h"
#include "Application/ReportService.h"
#include "Domain/Booking.h"
#include "Domain/RepositoryPattern.h"
#include "Domain/BookingRepository.h"
#include "Domain/OrderRepository.h"
#include "Infrastructure/FileInventoryRepository.h"
#include "Infrastructure/FileBookingRepository.h"
#include "Infrastructure/FileOrderRepository.h"

// ============================================================================
// BOOKING SERVICE TESTS
// ============================================================================

TEST(BookingServiceTests, CreateBookingSucceedsWithValidStock) {
    InMemoryInventoryRepository invRepo;
    invRepo.save(Product("Aspirin", 10, 5.0));
    
    FileBookingRepository bookingRepo("test_bookings_1.json");
    InventoryService inventory(invRepo);
    BookingService booking(bookingRepo, inventory);

    auto result = booking.createBooking("Aspirin", 3, "customer1");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getProductName(), "Aspirin");
    EXPECT_EQ(result->getQuantity(), 3);
    EXPECT_EQ(result->getCustomerId(), "customer1");
    EXPECT_EQ(result->getStatus(), BookingStatus::RESERVED);
}

TEST(BookingServiceTests, CreateBookingFailsWithInsufficientStock) {
    InMemoryInventoryRepository invRepo;
    invRepo.save(Product("Aspirin", 2, 5.0));
    
    FileBookingRepository bookingRepo("test_bookings_2.json");
    InventoryService inventory(invRepo);
    BookingService booking(bookingRepo, inventory);

    auto result = booking.createBooking("Aspirin", 5, "customer1");
    EXPECT_FALSE(result.has_value());
}

TEST(BookingServiceTests, ConfirmBookingChangesStatus) {
    InMemoryInventoryRepository invRepo;
    invRepo.save(Product("Aspirin", 10, 5.0));
    
    FileBookingRepository bookingRepo("test_bookings_3.json");
    InventoryService inventory(invRepo);
    BookingService booking(bookingRepo, inventory);

    auto createdBooking = booking.createBooking("Aspirin", 3, "customer1");
    ASSERT_TRUE(createdBooking.has_value());
    
    bool confirmed = booking.confirmBooking(createdBooking->getId());
    EXPECT_TRUE(confirmed);
    
    auto updatedBooking = bookingRepo.getById(createdBooking->getId());
    ASSERT_TRUE(updatedBooking.has_value());
    EXPECT_EQ(updatedBooking->getStatus(), BookingStatus::CONFIRMED);
}

TEST(BookingServiceTests, CancelBookingReleasesItems) {
    InMemoryInventoryRepository invRepo;
    invRepo.save(Product("Aspirin", 10, 5.0));
    
    FileBookingRepository bookingRepo("test_bookings_4.json");
    InventoryService inventory(invRepo);
    BookingService booking(bookingRepo, inventory);

    auto createdBooking = booking.createBooking("Aspirin", 3, "customer1");
    ASSERT_TRUE(createdBooking.has_value());
    
    auto beforeCancel = invRepo.getByName("Aspirin");
    ASSERT_TRUE(beforeCancel.has_value());
    
    bool cancelled = booking.cancelBooking(createdBooking->getId());
    EXPECT_TRUE(cancelled);
    
    auto afterCancel = invRepo.getByName("Aspirin");
    ASSERT_TRUE(afterCancel.has_value());
    EXPECT_EQ(afterCancel->getPackCount(), beforeCancel->getPackCount() + 3);
}

TEST(BookingServiceTests, GetActiveBookingsReturnsOnlyActiveBookings) {
    InMemoryInventoryRepository invRepo;
    invRepo.save(Product("Aspirin", 20, 5.0));
    
    FileBookingRepository bookingRepo("test_bookings_5.json");
    InventoryService inventory(invRepo);
    BookingService booking(bookingRepo, inventory);

    auto b1 = booking.createBooking("Aspirin", 3, "customer1");
    auto b2 = booking.createBooking("Aspirin", 2, "customer2");
    
    auto active = booking.getActiveBookings();
    EXPECT_EQ(active.size(), 2);
}

// ============================================================================
// PERSISTENCE / FILE STORAGE TESTS
// ============================================================================

TEST(FileInventoryRepositoryTests, SaveAndLoadInventory) {
    std::string testFile = "test_inventory_persistence.json";
    
    // Save
    {
        FileInventoryRepository repo(testFile);
        repo.save(Product("Medicine1", 5, 10.0));
        repo.save(Product("Medicine2", 3, 15.0));
    }
    
    // Load
    {
        FileInventoryRepository repo(testFile);
        auto med1 = repo.getByName("Medicine1");
        auto med2 = repo.getByName("Medicine2");
        
        ASSERT_TRUE(med1.has_value());
        ASSERT_TRUE(med2.has_value());
        EXPECT_EQ(med1->getPackCount(), 5);
        EXPECT_EQ(med1->getPrice(), 10.0);
        EXPECT_EQ(med2->getPackCount(), 3);
        EXPECT_EQ(med2->getPrice(), 15.0);
    }
}

TEST(FileBookingRepositoryTests, SaveAndLoadBookings) {
    std::string testFile = "test_booking_persistence.json";
    
    // Save
    {
        FileBookingRepository repo(testFile);
        auto expiresAt = std::chrono::system_clock::now() + std::chrono::hours(24);
        Booking b("BK001", "Aspirin", 5, "customer1", expiresAt);
        repo.save(b);
    }
    
    // Load
    {
        FileBookingRepository repo(testFile);
        auto loaded = repo.getById("BK001");
        
        ASSERT_TRUE(loaded.has_value());
        EXPECT_EQ(loaded->getProductName(), "Aspirin");
        EXPECT_EQ(loaded->getQuantity(), 5);
        EXPECT_EQ(loaded->getCustomerId(), "customer1");
    }
}

// ============================================================================
// DISCOUNT SERVICE TESTS
// ============================================================================

TEST(DiscountServiceTests, RegisterAndRetrieveDiscount) {
    DiscountService service;
    
    auto silverCard = service.getDiscountCard("SILVER");
    ASSERT_TRUE(silverCard.has_value());
    EXPECT_EQ(silverCard->getCardType(), "SILVER");
}

TEST(DiscountServiceTests, ApplyDiscountReducesPrice) {
    DiscountService service;
    
    double baseAmount = 100.0;
    double discounted = service.applyDiscount("SILVER", baseAmount);
    
    // SILVER is 10% discount
    EXPECT_LT(discounted, baseAmount);
    EXPECT_DOUBLE_EQ(discounted, 90.0);
}

TEST(DiscountServiceTests, GetAvailableCardsReturnsAll) {
    DiscountService service;
    
    auto cards = service.getAvailableCards();
    EXPECT_GE(cards.size(), 4);  // At least SILVER, GOLD, PLATINUM, BRONZE
}

// ============================================================================
// REPORT SERVICE TESTS
// ============================================================================

TEST(ReportServiceTests, GetOrderStatisticsReturnsValidData) {
    InMemoryInventoryRepository invRepo;
    FileOrderRepository orderRepo("test_order_stats.json");
    FileBookingRepository bookingRepo("test_booking_stats.json");
    
    ReportService report(orderRepo, bookingRepo, invRepo);
    
    auto stats = report.getOrderStatistics();
    EXPECT_GE(stats.totalOrders, 0);
    EXPECT_GE(stats.totalRevenue, 0.0);
}

TEST(ReportServiceTests, GetActiveBookingCountReturnsZeroWhenEmpty) {
    InMemoryInventoryRepository invRepo;
    FileOrderRepository orderRepo("test_order_empty.json");
    FileBookingRepository bookingRepo("test_booking_empty.json");
    
    ReportService report(orderRepo, bookingRepo, invRepo);
    
    int count = report.getActiveBookingCount();
    EXPECT_EQ(count, 0);
}

// ============================================================================
// BUSINESS RULES TESTS
// ============================================================================

TEST(BusinessRulesTests, BookingExpirationWorks) {
    InMemoryInventoryRepository invRepo;
    invRepo.save(Product("Aspirin", 10, 5.0));
    
    FileBookingRepository bookingRepo("test_expiration.json");
    InventoryService inventory(invRepo);
    BookingService booking(bookingRepo, inventory);

    // Create booking with immediate expiration
    auto expiresAt = std::chrono::system_clock::now() - std::chrono::seconds(1);
    std::string bookingId = "EXPIRED_BK";
    Booking expiredBooking(bookingId, "Aspirin", 3, "customer1", 
                           BookingStatus::RESERVED, 
                           std::chrono::system_clock::now(), expiresAt);
    bookingRepo.save(expiredBooking);
    
    EXPECT_TRUE(expiredBooking.isExpired());
}

TEST(BusinessRulesTests, InventoryDecreasesAfterSale) {
    InMemoryInventoryRepository repo;
    repo.save(Product("Vitamin", 100, 8.0));
    
    InventoryService inventory(repo);
    
    bool success = inventory.decreaseStock("Vitamin", 25);
    EXPECT_TRUE(success);
    
    auto updated = repo.getByName("Vitamin");
    ASSERT_TRUE(updated.has_value());
    EXPECT_EQ(updated->getPackCount(), 75);
}
