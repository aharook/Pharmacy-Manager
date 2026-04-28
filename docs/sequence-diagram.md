# Sequence Diagram

```mermaid
sequenceDiagram
    actor User as User
    participant UI as PharmacyConsole
    participant Order as Order
    participant Factory as SaleFactory
    participant Repo as FileOrderRepository

    User->>UI: Create order (product, qty, sale type)
    UI->>Order: addItem(product, qty)
    UI->>Order: calculateTotal()
    Order->>Factory: createSale(type)
    Factory-->>Order: Sale
    Order-->>UI: total
    UI->>Repo: save(order)
    Repo-->>UI: saved
    UI-->>User: Show order ID and total
```

## Сценарiй
Користувач створює замовлення у консолi. Система рахує суму через Factory (DIRECT або BOOKING) i зберiгає замовлення у файловому репозиторiї.

## Межi вiдповiдальностi
- Console збирає данi та показує результат.
- Order рахує суму через SaleFactory.
- FileOrderRepository зберiгає замовлення у файл.
