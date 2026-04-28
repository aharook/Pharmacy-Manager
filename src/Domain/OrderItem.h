#ifndef DOMAIN_ORDER_ITEM_H
#define DOMAIN_ORDER_ITEM_H

#include <stdexcept>
#include <utility>

#include "Product.h"

class OrderItem {
private:
    Product product_;
    int quantity_;
    double unitPrice_;
public:
    OrderItem(Product product, int quantity)
        : product_(std::move(product)), quantity_(quantity), unitPrice_(product_.getPrice()) {
        if (quantity_ <= 0) {
            throw std::invalid_argument("quantity must be positive");
        }
    }

    const Product& getProduct() const {return product_;}

    int getQuantity() const {return quantity_;}

    double getUnitPrice() const {return unitPrice_;}

    double getLineTotal() const {
        return unitPrice_ * static_cast<double>(quantity_);
    }
};

#endif
