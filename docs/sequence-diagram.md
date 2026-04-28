# Sequence Diagram

```mermaid
sequenceDiagram
    actor Cashier as Працівник
    participant UI as ConsoleUI
    participant Controller as PharmacyController
    participant Inventory as InventoryService
    participant Repo as IInventoryRepository
    participant OrderService as OrderService
    participant Discount as DiscountCard
    participant Strategy as IDiscountStrategy

    Cashier->>UI: Вибирає оформлення замовлення
    UI->>Controller: createOrder(items, discountCard)
    Controller->>Inventory: reserveItems(name, quantity)
    Inventory->>Repo: getByName(name)
    Repo-->>Inventory: Product
    Inventory-->>Controller: items reserved
    Controller->>OrderService: createOrder(items)
    OrderService->>OrderService: calculateTotal(order)
    OrderService->>Discount: getDiscountStrategy()
    Discount->>Strategy: calculate(baseAmount)
    Strategy-->>Discount: discountedAmount
    Discount-->>OrderService: discountedAmount
    OrderService-->>Controller: final order total
    Controller->>Inventory: decreaseStock(name, quantity)
    Inventory->>Repo: update(product)
    Repo-->>Inventory: saved
    Controller-->>UI: підтвердження продажу та сума
    UI-->>Cashier: Показати чек
```

## Сценарій
Головний сценарій для першої ітерації: працівник створює замовлення або продаж, система перевіряє наявність препарату, списує кількість зі складу та рахує підсумкову суму з урахуванням дисконтної карти.

## Межі відповідальності
- UI тільки збирає дані та показує результат.
- Бізнес-логіка перевіряє склад, створює замовлення і рахує суму.
- Інфраструктура зберігає та оновлює дані про препарати.
