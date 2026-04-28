#ifndef DOMAIN_ORDER_STATUS_H
#define DOMAIN_ORDER_STATUS_H

enum class OrderStatus {
    PENDING,      // Замовлення створено, очікує підтвердження
    CONFIRMED,    // Замовлення підтверджено, товар видаватиметься
    DELIVERED,    // Товар видан клієнту
    RETURNED,     // Товар повернено (частково або повністю)
    CANCELLED     // Замовлення скасовано
};

inline const char* orderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::PENDING: return "PENDING";
        case OrderStatus::CONFIRMED: return "CONFIRMED";
        case OrderStatus::DELIVERED: return "DELIVERED";
        case OrderStatus::RETURNED: return "RETURNED";
        case OrderStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

inline OrderStatus stringToOrderStatus(const std::string& str) {
    if (str == "CONFIRMED") return OrderStatus::CONFIRMED;
    if (str == "DELIVERED") return OrderStatus::DELIVERED;
    if (str == "RETURNED") return OrderStatus::RETURNED;
    if (str == "CANCELLED") return OrderStatus::CANCELLED;
    return OrderStatus::PENDING;
}

#endif
