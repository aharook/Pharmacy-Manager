#ifndef DOMAIN_ORDER_REPOSITORY_H
#define DOMAIN_ORDER_REPOSITORY_H

#include <stdexcept>
#include <string>
#include <vector>

#include "Order.h"

class IOrderRepository {
public:
    virtual ~IOrderRepository() = default;

    virtual void save(const Order& order) = 0;
    virtual const Order& getById(const std::string& id) const = 0;
    virtual std::vector<Order> getAll() const = 0;
    virtual void update(const Order& order) = 0;
    virtual void deleteById(const std::string& id) = 0;
};

#endif
