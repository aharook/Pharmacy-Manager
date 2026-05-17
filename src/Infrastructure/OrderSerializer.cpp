#include "OrderSerializer.h"

std::string OrderSerializer::serialize(const Order& order) const {
    std::string result;
    result += order.getId() + "|";
    result += std::to_string(static_cast<int>(order.getSaleType())) + "|";
    result += std::to_string(order.getTotal()) + "|";
    result += (order.isReceived() ? "1" : "0");
    return result;
}
