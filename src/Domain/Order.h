#ifndef DOMAIN_ORDER_H
#define DOMAIN_ORDER_H

#include <string>
#include <utility>
#include <vector>

#include "OrderItem.h"
#include "SaleFactory.h"

class Order {
private:
    std::string id_;
    std::vector<OrderItem> items_;
    double total_;
    SaleType saleType_;

public:
    Order() : id_(""), total_(0.0), saleType_(SaleType::DIRECT) {}

    Order(std::string id, SaleType saleType) : id_(std::move(id)), total_(0.0), saleType_(saleType) {}

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

    SaleType getSaleType() const {return saleType_;}

    void calculateTotal() {
        Sale* sale = SaleFactory::createSale(saleType_);
        total_ = sale->calculate(items_);
        delete sale;
    }
};

#endif
