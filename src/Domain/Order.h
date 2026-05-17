#ifndef DOMAIN_ORDER_H
#define DOMAIN_ORDER_H

#include <string>
#include <utility>
#include <vector>

#include "OrderItem.h"

enum class SaleType {
    DIRECT,   
    BOOKING  
};

class Order {
private:
    std::string id_;
    std::vector<OrderItem> items_;
    double total_;
    SaleType saleType_;
    bool isReceived_;

public:
    Order() : id_(""), total_(0.0), saleType_(SaleType::DIRECT), isReceived_(false) {}

    Order(std::string id, SaleType saleType) 
        : id_(std::move(id)), total_(0.0), saleType_(saleType), isReceived_(false) {}

    const std::string& getId() const { return id_; }

    void addItem(const OrderItem& item) {
        items_.push_back(item);
    }

    const std::vector<OrderItem>& getItems() const {
        return items_;
    }

    double getTotal() const {
        return total_;
    }

    void setTotal(double total) {
        total_ = total;
    }

    SaleType getSaleType() const { return saleType_; }

    bool isReceived() const { return isReceived_; }

    void markAsReceived() { isReceived_ = true; }
};

#endif
