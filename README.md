# Pharmacy Manager

Інтегрована система управління аптекою з підтримкою замовлень, бронювань, знижок та аналітики.

## Версія

**Lab 35** - Бізнес-логіка, Persistence та Запити

## Особливості

### Ітерація 1 (Lab 34) ✅
- Основні сутності (Product, Order, OrderItem)
- Базова бізнес-логіка замовлень
- Strategy Pattern для знижок
- Repository Pattern для складу
- 6 модульних тестів

### Ітерація 2 (Lab 35) ✅
- **Persistence:** JSON-файлове сховище для складу, замовлень, бронювань
- **Booking:** Система бронювання товарів з управлінням конфліктів
- **Discount Service:** Розширена система знижок з реєстрацією стратегій
- **Report Service:** Аналітика, пошук, фільтрація, сортування
- **Console Interface:** Повноцінне меню для всіх операцій
- **16 тестів** для нового функціоналу

### Ітерація 3 (Lab 36) - Планується
- Розширене тестування (minimum 20 тестів)
- Quality gate: цілісність, обробка помилок, edge cases
- Інтеграційне тестування
- Поліпшена обробка помилок

### Ітерація 4 (Lab 37) - Планується
- Релізна збірка
- Фінальна документація
- Демо-сценарій
- Вибірковий поліпшення persistence (можливо SQLite)

## Архітектура

```
Domain Layer
├── Product, Order, OrderItem
├── Booking, BookingStatus, OrderStatus
├── Return
└── Contracts: IInventoryRepository, IOrderRepository, IBookingRepository, IReturnRepository

Application Layer
├── InventoryService
├── OrderService
├── BookingService
├── DiscountService
├── ReportService
└── PharmacyController

Infrastructure Layer
├── FileStorageHelper (JSON парсер)
├── FileInventoryRepository
├── FileOrderRepository
├── FileBookingRepository
└── FileStorageHelper

Console Layer
└── PharmacyConsole (повноцінне меню)
```

## Встановлення і компіляція

### Вимоги
- CMake 3.10+
- C++17 компілятор
- GTest для тестування

### Компіляція
```bash
cd pharmacy
mkdir build
cd build
cmake ..
make
```

### Запуск програми
```bash
./PharmacyApp
```

### Запуск тестів
```bash
./PharmacyTests
```

## Основні бізнес-правила

### Замовлення
1. Замовлення можна створити тільки якщо товар є у складі
2. Знижка застосовується тільки якщо сума мінімум 10 одиниць
3. Замовлення отримує статус від PENDING до CANCELLED

### Бронювання
1. Період дійсності бронювання: 24 години
2. Один товар не може мати більше 5 одночасних бронювань
3. Прострочене бронювання можна скасувати
4. Скасування бронювання повертає товар у склад

### Знижки
- SILVER: 10% знижка
- GOLD: 20% знижка
- PLATINUM: 30% знижка
- BRONZE: 5 одиниць фіксованої знижки

## Persistence

Дані зберігаються у JSON форматі:
- `inventory.json` - склад
- `orders.json` - замовлення
- `bookings.json` - бронювання

При кожній операції дані автоматично зберігаються. Резервна копія (`.bak`) створюється перед оновленням.

## API Сервісів

### InventoryService
```cpp
std::optional<Product> findProduct(const std::string& name);
bool reserveItems(const std::string& name, int quantity);
bool decreaseStock(const std::string& name, int quantity);
bool releaseItems(const std::string& name, int quantity);
```

### BookingService
```cpp
std::optional<Booking> createBooking(const std::string& productName, 
                                      int quantity, 
                                      const std::string& customerId);
bool confirmBooking(const std::string& bookingId);
bool cancelBooking(const std::string& bookingId);
std::vector<Booking> getActiveBookings();
int cleanupExpiredBookings();
```

### DiscountService
```cpp
std::optional<DiscountCard> getDiscountCard(const std::string& cardType);
double applyDiscount(const std::string& cardType, double baseAmount);
std::vector<std::string> getAvailableCards();
double applyCombinedDiscount(const std::vector<std::string>& cardTypes, double baseAmount);
```

### ReportService
```cpp
OrderStats getOrderStatistics();
std::vector<ProductSalesStats> getProductSalesStats();
std::vector<std::string> getLowStockProducts(int threshold);
std::vector<ProductSalesStats> getTopSellingProducts(int limit);
std::vector<std::string> searchProductsByName(const std::string& query);
std::vector<std::string> getProductsInStock(int minQuantity);
```

## Тестування

### Запуск тестів
```bash
cd build
ctest -V
```

### Тестове покриття
- 6 тестів з Lab 34 (Product, Order, Inventory, Discount)
- 16 тестів з Lab 35 (Booking, Persistence, Discount Service, Report Service)

**Всього:** 22 тести

### Приклади тестів
```cpp
// Booking
TEST(BookingServiceTests, CreateBookingSucceedsWithValidStock)
TEST(BookingServiceTests, CancelBookingReleasesItems)
TEST(BookingServiceTests, GetActiveBookingsReturnsOnlyActiveBookings)

// Persistence
TEST(FileInventoryRepositoryTests, SaveAndLoadInventory)
TEST(FileBookingRepositoryTests, SaveAndLoadBookings)

// Discount Service
TEST(DiscountServiceTests, RegisterAndRetrieveDiscount)
TEST(DiscountServiceTests, ApplyDiscountReducesPrice)

// Report Service
TEST(ReportServiceTests, GetOrderStatisticsReturnsValidData)
TEST(ReportServiceTests, GetActiveBookingCountReturnsZeroWhenEmpty)
```

## Консольний інтерфейс

При запуску програми з'являється основне меню:

```
========== PHARMACY MANAGER ==========
1. Управління складом
2. Замовлення
3. Бронювання
4. Знижки
5. Звіти та аналітика
6. Збереження даних
0. Вихід
```

Кожна секція містить свої підменю для операцій.

## Бізнес-логіка і точки розширення

### Використаний паттерн: Strategy Pattern

**Для знижок:**
- `IDiscountStrategy` - контракт
- `PercentageDiscountStrategy` - реалізація для відсоткових знижок
- `FixedDiscountStrategy` - реалізація для фіксованих сум

Дозволяє легко додати нові типи знижок без зміни сервісу.

### Можливі розширення (Lab 37)

1. **Комбіновані знижки** - застосування кількох стратегій одночасно
2. **Часові знижки** - знижка залежить від часу
3. **Умовні знижки** - знижка залежить від суми або кількості
4. **Нові канали persistence** - SQLite замість JSON
5. **Event-driven** - Observer Pattern для подій замовлення

## Файлова структура

```
pharmacy/
├── src/
│   ├── Domain/          # Доменні сутності та контракти
│   ├── Application/     # Бізнес-логіка та сервіси
│   ├── Infrastructure/  # Реалізація persistence
│   └── Console/         # UI
├── tests/               # Тести
├── docs/                # Документація
├── CMakeLists.txt       # Конфігурація
└── data/                # JSON файли з даними
```

## Документація

- [Vision](docs/vision.md) - Загальне бачення проекту
- [Backlog](docs/backlog.md) - План розвитку по ітераціям
- [Iteration 2 Plan](docs/iteration-2-plan.md) - Планування Lab 35
- [Iteration 2](docs/iteration-2.md) - Звіт про реалізацію Lab 35
- [Class Diagram](docs/class-diagram.md) - Діаграми (потребує оновлення)
- [Sequence Diagram](docs/sequence-diagram.md) - Послідовність операцій (потребує оновлення)

## Контрибутори

Розроблено для навчального курсу "Проектування ПЗ" (ОПІ/ОПАМ).

## Ліцензія

MIT License
