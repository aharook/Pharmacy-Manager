#ifndef INFRASTRUCTURE_FILE_ORDER_REPOSITORY_H
#define INFRASTRUCTURE_FILE_ORDER_REPOSITORY_H

#include <string>
#include <unordered_map>

#include "../Domain/OrderRepository.h"

class FileOrderRepository : public IOrderRepository {
private:
    std::string filePath_;
    std::unordered_map<std::string, Order> cache_;
    bool isDirty_;

public:
    explicit FileOrderRepository(const std::string& filePath)
        : filePath_(filePath), isDirty_(false) {
        loadFromFile();
    }

    ~FileOrderRepository() {
        if (isDirty_) {
            try {
                saveToFile();
            } catch (...) {
                // Error handling on destruction
            }
        }
    }

    void save(const Order& order) override;
    std::optional<Order> getById(const std::string& id) const override;
    std::vector<Order> getAll() const override;
    std::vector<Order> getByStatus(OrderStatus status) const override;
    void update(const Order& order) override;
    void deleteById(const std::string& id) override;

    // Persistence operations
    void saveToFile();
    void loadFromFile();

private:
    std::string serializeOrder(const Order& order) const;
    Order deserializeOrder(const std::string& json) const;
};

#endif
