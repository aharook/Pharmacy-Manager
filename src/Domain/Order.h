#ifndef DOMAIN_ORDER_H
#define DOMAIN_ORDER_H

#include <vector>

#include "OrderItem.h"



class Order {
private:
    std::vector<OrderItem> items_;
    double total_ = 0.0;
    
public:
    void addItem(const OrderItem& item) {
        items_.push_back(item);
    }

    const std::vector<OrderItem>& getItems() const {
        return items_;
    }

    double getTotal() const {
        return total_;
    }

    double calculateTotal() {
        total_ = 0.0;
        for (const auto& item : items_) {
            total_ += item.getLineTotal();
        }
        return total_;
    }
};

#endif
