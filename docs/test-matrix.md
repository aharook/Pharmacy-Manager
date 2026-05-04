# Test Matrix

| Use case | Тести |
| --- | --- |
| Перегляд складу | [tests/test_e2e.cpp](tests/test_e2e.cpp) (CreateOrderAndBookingFlow), [tests/test_edge_cases.cpp](tests/test_edge_cases.cpp) (AddProductZeroQuantityAndPrice) |
| Створення DIRECT продажу з перевiркою залишку | [tests/test_business_logic.cpp](tests/test_business_logic.cpp) (DirectSaleCalculation, CreateOrderAndCalculateTotal) |
| Створення BOOKING продажу з нацiнкою 15% | [tests/test_business_logic.cpp](tests/test_business_logic.cpp) (BookingSaleCalculation), [tests/test_e2e.cpp](tests/test_e2e.cpp) (CreateOrderAndBookingFlow) |
| Створення бронювання | [tests/test_e2e.cpp](tests/test_e2e.cpp) (CreateOrderAndBookingFlow), [tests/test_error_cases.cpp](tests/test_error_cases.cpp) (CreateBookingEmptyOrderId) |
| Позначення бронювання як пропущене | [tests/test_business_logic.cpp](tests/test_business_logic.cpp) (MissedBookingPenalty), [tests/test_e2e.cpp](tests/test_e2e.cpp) (CreateOrderAndBookingFlow), [tests/test_error_cases.cpp](tests/test_error_cases.cpp) (MarkBookingNotFound) |

