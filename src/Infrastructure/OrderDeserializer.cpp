#include "OrderDeserializer.h"
#include <sstream>

Order OrderDeserializer::deserialize(const std::string& line) const {
    std::stringstream ss(line);
    std::string id, saleTypeStr, totalStr, receivedStr;

    if (!std::getline(ss, id, '|') || 
        !std::getline(ss, saleTypeStr, '|') || 
        !std::getline(ss, totalStr, '|')) {
        throw std::runtime_error("Invalid order format");
    }

    int saleTypeInt = std::stoi(saleTypeStr);
    double total = std::stod(totalStr);
    SaleType saleType = (saleTypeInt == 0) ? SaleType::DIRECT : SaleType::BOOKING;

    Order order(id, saleType);
    order.setTotal(total);

    if (std::getline(ss, receivedStr, '|') && receivedStr == "1") {
        order.markAsReceived();
    }

    return order;
}
