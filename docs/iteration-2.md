# Iteration 2 (Simplified)


## Що реалiзовано


### Factory Pattern
- Додано `SaleFactory` для створення типiв продажiв:
  - DIRECT: без нацiнки
  - BOOKING: нацiнка 15%

### Repository Pattern
- Залишено `OrderRepository` як контракт.
- Реалiзовано `FileOrderRepository` з простим текстовим форматом `id|saleType|total`.

### Консольний iнтерфейс
- Спрощене меню: склад, замовлення, бронювання.
- Створення замовлень та бронювань, перегляд результатiв.

### Тести
- Додано 12 юнiт-тестiв для ключових правил:
  - Product, OrderItem, SaleFactory, Order, Booking, FileOrderRepository.

### Entry Point
- Додано `src/main.cpp`, який iнiцiалiзує `FileOrderRepository` i запускає `PharmacyConsole`.

## Результат
- Проєкт має компактну структуру, сфокусовану на двох патернах.
- Вся функцiональнiсть працює у консольному режимi.
- Збереження замовлень працює без стороннiх залежностей.
- 12 тестiв проходять успiшно.
