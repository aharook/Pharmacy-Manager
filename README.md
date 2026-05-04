# Pharmacy Manager 

Освiтнiй мiнi-проєкт для демонстрацiї патернiв Factory та Repository. Система керує товарами, замовленнями та бронюваннями з простим файловим збереженням.

## Можливостi
- Перегляд складу.
- Створення DIRECT/BOOKING замовлень з розрахунком суми.
- Бронювання зi штрафом 20% при пропуску.
- Збереження замовлень у файл `pharmacy_orders.txt` у форматi `id|saleType|total`.

## Архiтектура
```
Aplication
├── BoockingService
├── InventoryService
└── OrderService
Domain
├── Product, OrderItem, Order
├── Booking
├── SaleFactory (Factory Pattern)
└── OrderRepository (contract)

Infrastructure
├── FileOrderRepository 
├── FileProductRepository

Console
├── BookingManager
├── InventoryManager
├── OrderManager
└── PharmacyConsole

Entry Point
└── main.cpp
```

## Збирання та запуск
### Вимоги
- CMake 3.20+
- C++17 компiлятор
- GTest для тестiв

### Збирання
```bash
cmake -B build
cmake --build build
```

### Запуск програми
```bash
./build/Debug/pharmacy.exe
```

### Запуск тестiв
```bash
./build/Debug/pharmacy_tests.exe
```

## Бiзнес-правила
### Замовлення
1. Замовлення створюється лише за наявностi товару на складi.
2. DIRECT продаж рахує суму без нацiнки.
3. BOOKING продаж має нацiнку 15%.

### Бронювання
1. Бронювання може бути позначене як пропущене.
2. Якщо пропущено, застосовується штраф 20% (сума * 0.8).



## Документацiя
- [Vision](docs/vision.md)
- [Backlog](docs/backlog.md)
- [Iteration 2](docs/iteration-2.md)
- [Class Diagram](docs/class-diagram.md)
- [Sequence Diagram](docs/sequence-diagram.md)
- [Iteration 3](docs/iteration-3.md)
- [TESTING](docs/TESTING.md)
- [test-matrix](docs/test-matrix)
- [test-strategy](docs/test-strategy)

