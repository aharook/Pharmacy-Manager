#ifndef DOMAIN_PRODUCT_H
#define DOMAIN_PRODUCT_H

#include <stdexcept>
#include <string>
#include <utility>


class Product {
private:
    std::string name_;
    int packCount_;
    double price_;
public:
    Product() : name_(""), packCount_(0), price_(0.0) {}

    Product(std::string name, int packCount, double price)
        : name_(std::move(name)), packCount_(packCount), price_(price) {
        if (packCount_ < 0) {
            throw std::invalid_argument("packCount must be non-negative");
        }
        if (price_ < 0.0) {
            throw std::invalid_argument("price must be non-negative");
        }
    }

    const std::string& getName() const {
        return name_;
    }

    int getPackCount() const {
        return packCount_;
    }

    double getPrice() const {
        return price_;
    }

    void decreasePackCount(int quantity) {
        if (quantity < 0) {
            throw std::invalid_argument("quantity must be non-negative");
        }
        if (quantity > packCount_) {
            throw std::invalid_argument("not enough stock");
        }
        packCount_ -= quantity;
    }

    void increasePackCount(int quantity) {
        if (quantity < 0) {
            throw std::invalid_argument("quantity must be non-negative");
        }
        packCount_ += quantity;
    }
};

#endif
