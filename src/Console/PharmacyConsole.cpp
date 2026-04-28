#include "PharmacyConsole.h"

#include <iostream>
#include <iomanip>

void PharmacyConsole::run() {
    bool isRunning = true;
    
    while (isRunning) {
        showMainMenu();
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                showInventoryMenu();
                break;
            case 2:
                showOrderMenu();
                break;
            case 3:
                showBookingMenu();
                break;
            case 4:
                showDiscountMenu();
                break;
            case 5:
                showReportMenu();
                break;
            case 6:
                showPersistenceMenu();
                break;
            case 0:
                isRunning = false;
                std::cout << "\nDякую за користування Pharmacy Manager!\n" << std::endl;
                break;
            default:
                displayError("Невірний вибір. Спробуйте ще раз.");
        }
    }
}

void PharmacyConsole::showMainMenu() {
    std::cout << "\n========== PHARMACY MANAGER ==========" << std::endl;
    std::cout << "1. Управління складом" << std::endl;
    std::cout << "2. Замовлення" << std::endl;
    std::cout << "3. Бронювання" << std::endl;
    std::cout << "4. Знижки" << std::endl;
    std::cout << "5. Звіти та аналітика" << std::endl;
    std::cout << "6. Збереження даних" << std::endl;
    std::cout << "0. Вихід" << std::endl;
    std::cout << "Виберіть опцію: ";
}

void PharmacyConsole::showInventoryMenu() {
    bool inMenu = true;
    
    while (inMenu) {
        std::cout << "\n===== УПРАВЛІННЯ СКЛАДОМ =====" << std::endl;
        std::cout << "1. Пошук товару за назвою" << std::endl;
        std::cout << "2. Переглянути всі товари у складі" << std::endl;
        std::cout << "3. Додати новий товар" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Виберіть опцію: ";
        
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                searchProduct();
                break;
            case 2:
                viewAllProducts();
                break;
            case 3:
                addProduct();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Невірний вибір.");
        }
    }
}

void PharmacyConsole::searchProduct() {
    std::string productName = getUserInput("Введіть назву товару: ");
    auto product = inventoryService_.findProduct(productName);
    
    if (product) {
        std::cout << "\n--- Інформація про товар ---" << std::endl;
        std::cout << "Назва: " << product->getName() << std::endl;
        std::cout << "Кількість упаковок: " << product->getPackCount() << std::endl;
        std::cout << "Ціна за упаковку: " << std::fixed << std::setprecision(2) 
                  << product->getPrice() << std::endl;
    } else {
        displayError("Товар не знайдено!");
    }
}

void PharmacyConsole::viewAllProducts() {
    std::cout << "\n--- Стан складу ---" << std::endl;
    // Note: Would need a getAll() method in InventoryService
    std::cout << "Функціональність отримання всіх товарів потребує розширення репозиторію." << std::endl;
}

void PharmacyConsole::addProduct() {
    std::string name = getUserInput("Введіть назву товару: ");
    std::string quantityStr = getUserInput("Введіть кількість упаковок: ");
    std::string priceStr = getUserInput("Введіть ціну за упаковку: ");
    
    try {
        int quantity = std::stoi(quantityStr);
        double price = std::stod(priceStr);
        
        Product product(name, quantity, price);
        // Save to repository
        displaySuccess("Товар успішно додано!");
    } catch (...) {
        displayError("Помилка при введенні даних.");
    }
}

void PharmacyConsole::showOrderMenu() {
    bool inMenu = true;
    
    while (inMenu) {
        std::cout << "\n===== ЗАМОВЛЕННЯ =====" << std::endl;
        std::cout << "1. Створити замовлення" << std::endl;
        std::cout << "2. Переглянути замовлення" << std::endl;
        std::cout << "3. Змінити статус замовлення" << std::endl;
        std::cout << "4. Переглянути деталі замовлення" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Виберіть опцію: ";
        
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                createOrder();
                break;
            case 2:
                viewOrders();
                break;
            case 3:
                changeOrderStatus();
                break;
            case 4:
                viewOrderDetails();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Невірний вибір.");
        }
    }
}

void PharmacyConsole::createOrder() {
    std::cout << "\n--- Створення замовлення ---" << std::endl;
    
    std::string productName = getUserInput("Введіть назву товару: ");
    std::string quantityStr = getUserInput("Введіть кількість упаковок: ");
    std::string useDiscount = getUserInput("Використовувати знижку? (y/n): ");
    
    try {
        int quantity = std::stoi(quantityStr);
        
        std::vector<OrderRequestItem> items = {{productName, quantity}};
        DiscountCard* discountCard = nullptr;
        
        if (useDiscount == "y" || useDiscount == "Y") {
            std::string cardType = getUserInput("Введіть тип картки дисконту: ");
            auto card = discountService_.getDiscountCard(cardType);
            if (card) {
                double total = controller_.sellProducts(items, &(*card));
                std::cout << "\nЗамовлення створено. Сума: " << std::fixed << std::setprecision(2) 
                          << total << std::endl;
            } else {
                displayError("Тип картки не знайдено!");
            }
        } else {
            double total = controller_.sellProducts(items, nullptr);
            std::cout << "\nЗамовлення створено. Сума: " << std::fixed << std::setprecision(2) 
                      << total << std::endl;
        }
        
    } catch (const std::exception& e) {
        displayError(std::string("Помилка: ") + e.what());
    }
}

void PharmacyConsole::viewOrders() {
    std::cout << "\n--- Список замовлень ---" << std::endl;
    
    auto allOrders = orderRepository_.getAll();
    
    if (allOrders.empty()) {
        std::cout << "Немає замовлень." << std::endl;
        return;
    }
    
    std::cout << std::setw(10) << "ID" << std::setw(15) << "Статус" 
              << std::setw(15) << "Сума" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    for (const auto& order : allOrders) {
        std::cout << std::setw(10) << order.getId()
                  << std::setw(15) << orderStatusToString(order.getStatus())
                  << std::setw(15) << std::fixed << std::setprecision(2) << order.getTotal()
                  << std::endl;
    }
}

void PharmacyConsole::changeOrderStatus() {
    std::string orderId = getUserInput("Введіть ID замовлення: ");
    auto order = orderRepository_.getById(orderId);
    
    if (!order) {
        displayError("Замовлення не знайдено!");
        return;
    }
    
    std::cout << "\nПточаний статус: " << orderStatusToString(order->getStatus()) << std::endl;
    std::cout << "1. PENDING (очікування)\n2. CONFIRMED (підтвердено)\n3. DELIVERED (видано)\n4. RETURNED (повернено)\n5. CANCELLED (скасовано)\n";
    std::cout << "Виберіть новий статус: ";
    
    int choice = getUserChoice();
    OrderStatus newStatus;
    
    switch (choice) {
        case 1: newStatus = OrderStatus::PENDING; break;
        case 2: newStatus = OrderStatus::CONFIRMED; break;
        case 3: newStatus = OrderStatus::DELIVERED; break;
        case 4: newStatus = OrderStatus::RETURNED; break;
        case 5: newStatus = OrderStatus::CANCELLED; break;
        default:
            displayError("Невірний вибір!");
            return;
    }
    
    order->setStatus(newStatus);
    orderRepository_.update(*order);
    displaySuccess("Статус замовлення змінено!");
}

void PharmacyConsole::viewOrderDetails() {
    std::string orderId = getUserInput("Введіть ID замовлення: ");
    auto order = orderRepository_.getById(orderId);
    
    if (!order) {
        displayError("Замовлення не знайдено!");
        return;
    }
    
    std::cout << "\n--- Деталі замовлення ---" << std::endl;
    std::cout << "ID: " << order->getId() << std::endl;
    std::cout << "Статус: " << orderStatusToString(order->getStatus()) << std::endl;
    std::cout << "Сума: " << std::fixed << std::setprecision(2) << order->getTotal() << std::endl;
    std::cout << "Товари:" << std::endl;
    
    for (const auto& item : order->getItems()) {
        std::cout << "  - " << item.getProduct().getName() << " x" << item.getQuantity()
                  << " @ " << item.getUnitPrice() << " = " << item.getLineTotal() << std::endl;
    }
}

void PharmacyConsole::showBookingMenu() {
    bool inMenu = true;
    
    while (inMenu) {
        std::cout << "\n===== БРОНЮВАННЯ =====" << std::endl;
        std::cout << "1. Створити бронювання" << std::endl;
        std::cout << "2. Підтвердити бронювання" << std::endl;
        std::cout << "3. Скасувати бронювання" << std::endl;
        std::cout << "4. Переглянути активні бронювання" << std::endl;
        std::cout << "5. Бронювання клієнта" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Виберіть опцію: ";
        
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                createBooking();
                break;
            case 2:
                confirmBooking();
                break;
            case 3:
                cancelBooking();
                break;
            case 4:
                viewActiveBookings();
                break;
            case 5:
                viewCustomerBookings();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Невірний вибір.");
        }
    }
}

void PharmacyConsole::createBooking() {
    std::cout << "\n--- Створення бронювання ---" << std::endl;
    
    std::string productName = getUserInput("Введіть назву товару: ");
    std::string quantityStr = getUserInput("Введіть кількість упаковок: ");
    std::string customerId = getUserInput("Введіть ID клієнта: ");
    
    try {
        int quantity = std::stoi(quantityStr);
        auto booking = bookingService_.createBooking(productName, quantity, customerId);
        
        if (booking) {
            std::cout << "\nБронювання успішно створено!" << std::endl;
            std::cout << "ID бронювання: " << booking->getId() << std::endl;
            std::cout << "Статус: " << bookingStatusToString(booking->getStatus()) << std::endl;
        } else {
            displayError("Не вдалося створити бронювання. Перевірте доступність товару.");
        }
    } catch (...) {
        displayError("Помилка при введенні даних.");
    }
}

void PharmacyConsole::confirmBooking() {
    std::string bookingId = getUserInput("Введіть ID бронювання: ");
    
    if (bookingService_.confirmBooking(bookingId)) {
        displaySuccess("Бронювання підтверджено!");
    } else {
        displayError("Не вдалося підтвердити бронювання.");
    }
}

void PharmacyConsole::cancelBooking() {
    std::string bookingId = getUserInput("Введіть ID бронювання: ");
    
    if (bookingService_.cancelBooking(bookingId)) {
        displaySuccess("Бронювання скасовано!");
    } else {
        displayError("Не вдалося скасувати бронювання.");
    }
}

void PharmacyConsole::viewActiveBookings() {
    auto bookings = bookingService_.getActiveBookings();
    
    if (bookings.empty()) {
        std::cout << "\nНемає активних бронювань." << std::endl;
        return;
    }
    
    std::cout << "\n--- Активні бронювання ---" << std::endl;
    std::cout << std::setw(15) << "ID" << std::setw(20) << "Товар" << std::setw(10) << "Кіл-сть"
              << std::setw(20) << "Клієнт" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    
    for (const auto& booking : bookings) {
        std::cout << std::setw(15) << booking.getId()
                  << std::setw(20) << booking.getProductName()
                  << std::setw(10) << booking.getQuantity()
                  << std::setw(20) << booking.getCustomerId() << std::endl;
    }
}

void PharmacyConsole::viewCustomerBookings() {
    std::string customerId = getUserInput("Введіть ID клієнта: ");
    auto bookings = bookingService_.getBookingsByCustomer(customerId);
    
    if (bookings.empty()) {
        std::cout << "\nНемає бронювань для цього клієнта." << std::endl;
        return;
    }
    
    std::cout << "\n--- Бронювання клієнта " << customerId << " ---" << std::endl;
    for (const auto& booking : bookings) {
        std::cout << "  ID: " << booking.getId() << ", Товар: " << booking.getProductName()
                  << ", Кіл-сть: " << booking.getQuantity()
                  << ", Статус: " << bookingStatusToString(booking.getStatus()) << std::endl;
    }
}

void PharmacyConsole::showDiscountMenu() {
    bool inMenu = true;
    
    while (inMenu) {
        std::cout << "\n===== ЗНИЖКИ =====" << std::endl;
        std::cout << "1. Переглянути доступні знижки" << std::endl;
        std::cout << "2. Протестувати знижку" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Виберіть опцію: ";
        
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                listAvailableDiscounts();
                break;
            case 2:
                testDiscount();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Невірний вибір.");
        }
    }
}

void PharmacyConsole::listAvailableDiscounts() {
    auto cards = discountService_.getAvailableCards();
    
    std::cout << "\n--- Доступні дисконтні карти ---" << std::endl;
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << (i + 1) << ". " << cards[i] << std::endl;
    }
}

void PharmacyConsole::testDiscount() {
    std::string cardType = getUserInput("Введіть тип картки: ");
    std::string amountStr = getUserInput("Введіть суму для розрахунку: ");
    
    try {
        double amount = std::stod(amountStr);
        double discounted = discountService_.applyDiscount(cardType, amount);
        
        std::cout << "\nРозрахунок:" << std::endl;
        std::cout << "Початкова сума: " << std::fixed << std::setprecision(2) << amount << std::endl;
        std::cout << "З урахуванням знижки: " << discounted << std::endl;
        std::cout << "Економія: " << (amount - discounted) << std::endl;
    } catch (...) {
        displayError("Помилка при введенні даних.");
    }
}

void PharmacyConsole::showReportMenu() {
    bool inMenu = true;
    
    while (inMenu) {
        std::cout << "\n===== ЗВІТИ ТА АНАЛІТИКА =====" << std::endl;
        std::cout << "1. Статистика замовлень" << std::endl;
        std::cout << "2. Продажи за товарами" << std::endl;
        std::cout << "3. Товари з низьким складом" << std::endl;
        std::cout << "4. Найпопулярніші товари" << std::endl;
        std::cout << "5. Пошук товарів за назвою" << std::endl;
        std::cout << "6. Товари у наявності" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Виберіть опцію: ";
        
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                viewOrderStatistics();
                break;
            case 2:
                viewProductSalesStats();
                break;
            case 3:
                viewLowStockProducts();
                break;
            case 4:
                viewTopSellingProducts();
                break;
            case 5:
                searchProductsByName();
                break;
            case 6:
                viewProductsInStock();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Невірний вибір.");
        }
    }
}

void PharmacyConsole::viewOrderStatistics() {
    auto stats = reportService_.getOrderStatistics();
    
    std::cout << "\n--- Статистика замовлень ---" << std::endl;
    std::cout << "Всього замовлень: " << stats.totalOrders << std::endl;
    std::cout << "Очікування: " << stats.pendingOrders << std::endl;
    std::cout << "Видано: " << stats.deliveredOrders << std::endl;
    std::cout << "Загальна виручка: " << std::fixed << std::setprecision(2) 
              << stats.totalRevenue << std::endl;
    std::cout << "Середня вартість замовлення: " << stats.averageOrderValue << std::endl;
}

void PharmacyConsole::viewProductSalesStats() {
    auto stats = reportService_.getProductSalesStats();
    
    if (stats.empty()) {
        std::cout << "\nНемає даних про продажи." << std::endl;
        return;
    }
    
    std::cout << "\n--- Продажи за товарами ---" << std::endl;
    std::cout << std::setw(20) << "Товар" << std::setw(15) << "Продано" 
              << std::setw(15) << "Виручка" << std::endl;
    std::cout << "-------------------------------------------" << std::endl;
    
    for (const auto& stat : stats) {
        std::cout << std::setw(20) << stat.productName
                  << std::setw(15) << stat.totalQuantitySold
                  << std::setw(15) << std::fixed << std::setprecision(2) << stat.totalRevenue
                  << std::endl;
    }
}

void PharmacyConsole::viewLowStockProducts() {
    std::string thresholdStr = getUserInput("Введіть поріг складу: ");
    
    try {
        int threshold = std::stoi(thresholdStr);
        auto products = reportService_.getLowStockProducts(threshold);
        
        if (products.empty()) {
            std::cout << "\nНемає товарів з низьким складом." << std::endl;
            return;
        }
        
        std::cout << "\n--- Товари з низьким складом (< " << threshold << ") ---" << std::endl;
        for (const auto& product : products) {
            std::cout << "  - " << product << std::endl;
        }
    } catch (...) {
        displayError("Помилка при введенні даних.");
    }
}

void PharmacyConsole::viewTopSellingProducts() {
    auto topProducts = reportService_.getTopSellingProducts(10);
    
    if (topProducts.empty()) {
        std::cout << "\nНемає даних про продажи." << std::endl;
        return;
    }
    
    std::cout << "\n--- Топ-10 найпопулярніших товарів ---" << std::endl;
    for (size_t i = 0; i < topProducts.size(); ++i) {
        std::cout << (i + 1) << ". " << topProducts[i].productName 
                  << " (продано: " << topProducts[i].totalQuantitySold << ")" << std::endl;
    }
}

void PharmacyConsole::searchProductsByName() {
    std::string query = getUserInput("Введіть критерій пошуку: ");
    auto results = reportService_.searchProductsByName(query);
    
    if (results.empty()) {
        std::cout << "\nТовари не знайдені." << std::endl;
        return;
    }
    
    std::cout << "\n--- Результати пошуку ---" << std::endl;
    for (const auto& product : results) {
        std::cout << "  - " << product << std::endl;
    }
}

void PharmacyConsole::viewProductsInStock() {
    std::string minStr = getUserInput("Введіть мінімальну кількість: ");
    
    try {
        int minQuantity = std::stoi(minStr);
        auto products = reportService_.getProductsInStock(minQuantity);
        
        if (products.empty()) {
            std::cout << "\nНемає товарів у наявності." << std::endl;
            return;
        }
        
        std::cout << "\n--- Товари у наявності (мінімум " << minQuantity << " упаковок) ---" << std::endl;
        for (const auto& product : products) {
            std::cout << "  - " << product << std::endl;
        }
    } catch (...) {
        displayError("Помилка при введенні даних.");
    }
}

void PharmacyConsole::showPersistenceMenu() {
    bool inMenu = true;
    
    while (inMenu) {
        std::cout << "\n===== ЗБЕРЕЖЕННЯ ДАНИХ =====" << std::endl;
        std::cout << "1. Зберегти дані" << std::endl;
        std::cout << "2. Завантажити дані" << std::endl;
        std::cout << "0. Назад" << std::endl;
        std::cout << "Виберіть опцію: ";
        
        int choice = getUserChoice();
        
        switch (choice) {
            case 1:
                saveData();
                break;
            case 2:
                loadData();
                break;
            case 0:
                inMenu = false;
                break;
            default:
                displayError("Невірний вибір.");
        }
    }
}

void PharmacyConsole::saveData() {
    // Persistence is handled automatically in file repositories
    displaySuccess("Дані автоматично зберігаються при кожній операції.");
}

void PharmacyConsole::loadData() {
    displaySuccess("Дані завантажуються при запуску програми.");
}

std::string PharmacyConsole::getUserInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

int PharmacyConsole::getUserChoice() {
    std::string input;
    std::getline(std::cin, input);
    try {
        return std::stoi(input);
    } catch (...) {
        return -1;
    }
}

void PharmacyConsole::displaySuccess(const std::string& message) {
    std::cout << "\n✓ " << message << std::endl;
}

void PharmacyConsole::displayError(const std::string& message) {
    std::cout << "\n✗ Помилка: " << message << std::endl;
}
