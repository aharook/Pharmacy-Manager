# Class Diagram

```mermaid
classDiagram
    direction LR


    class PharmacyController {
        +createOrder()
        +sellProducts()
    }

    class InventoryService {
        +findProduct(name)
        +reserveItems(name, quantity)
        +releaseItems(name, quantity)
        +decreaseStock(name, quantity)
    }

    class OrderService {
        +createOrder(items)
        +calculateTotal(order)
        +finalizeOrder(order)
    }

    class Product {
        +name: string
        +packCount: int
        +price: double
    }

    class Order {
        +items: vector~OrderItem~
        +total: double
    }

    class OrderItem {
        +quantity: int
        +unitPrice: double
    }

    namespace StrategyPattern {
        class DiscountCard {
            +cardType: string
            +discountStrategy: IDiscountStrategy
        }

        class IDiscountStrategy {
            <<interface>>
            +calculate(baseAmount)
        }

        class PercentageDiscountStrategy {
            +calculate(baseAmount)
        }

        class FixedDiscountStrategy {
            +calculate(baseAmount)
        }
    }

    namespace RepositoryPattern {
        class IInventoryRepository {
            <<interface>>
            +getByName(name)
            +save(product)
            +update(product)
        }

        class InMemoryInventoryRepository {
            +getByName(name)
            +save(product)
            +update(product)
        }
    }


    PharmacyController --> InventoryService
    PharmacyController --> OrderService
    InventoryService --> IInventoryRepository
    InMemoryInventoryRepository ..|> IInventoryRepository
    OrderService --> Order
    Order *-- OrderItem
    OrderItem --> Product
    DiscountCard --> IDiscountStrategy
    PercentageDiscountStrategy ..|> IDiscountStrategy
    FixedDiscountStrategy ..|> IDiscountStrategy
    OrderService --> DiscountCard
    InventoryService --> Product
```

## Патерни
- Strategy: `IDiscountStrategy` дозволяє додавати нові типи знижок без зміни сервісів.
- Repository: `IInventoryRepository` відокремлює роботу зі складом від бізнес-логіки.
