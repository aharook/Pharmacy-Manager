#ifndef DOMAIN_ORDER_H
#define DOMAIN_ORDER_H

#include <chrono>
#include <string>
#include <utility>
#include <vector>

#include "OrderItem.h"
#include "OrderStatus.h"



class Order {
private:
    std::string id_;
    std::vector<OrderItem> items_;
    double total_ = 0.0;
    OrderStatus status_;
    std::chrono::system_clock::time_point createdAt_;
    
public:
    Order() : id_(""), status_(OrderStatus::PENDING), 
              createdAt_(std::chrono::system_clock::now()) {}
    
    Order(std::string id) 
        : id_(std::move(id)), status_(OrderStatus::PENDING),
          createdAt_(std::chrono::system_clock::now()) {}

    // Для десеріалізації з файлу
    Order(std::string id, const std::vector<OrderItem>& items, 
          double total, OrderStatus status,
          std::chrono::system_clock::time_point createdAt)
        : id_(std::move(id)), items_(items), total_(total),
          status_(status), createdAt_(createdAt) {}

    const std::string& getId() const { return id_; }
    void setId(const std::string& id) { id_ = id; }

    void addItem(const OrderItem& item) {
        items_.push_back(item);
    }

    const std::vector<OrderItem>& getItems() const {
        return items_;
    }

    double getTotal() const {
        return total_;
    }

    OrderStatus getStatus() const { return status_; }
    
    void setStatus(OrderStatus newStatus) {
        status_ = newStatus;
    }

    std::chrono::system_clock::time_point getCreatedAt() const {
        return createdAt_;
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
