#ifndef INFRASTRUCTURE_ORDER_DESERIALIZER_H
#define INFRASTRUCTURE_ORDER_DESERIALIZER_H

#include <string>
#include "../Domain/Order.h"

class OrderDeserializer {
public:
    Order deserialize(const std::string& line) const;
};

#endif
