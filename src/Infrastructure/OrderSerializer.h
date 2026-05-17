#ifndef INFRASTRUCTURE_ORDER_SERIALIZER_H
#define INFRASTRUCTURE_ORDER_SERIALIZER_H

#include <string>
#include "../Domain/Order.h"

class OrderSerializer {
public:
    std::string serialize(const Order& order) const;
};

#endif
