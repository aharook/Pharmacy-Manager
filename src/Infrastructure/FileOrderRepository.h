#ifndef INFRASTRUCTURE_FILE_ORDER_REPOSITORY_H
#define INFRASTRUCTURE_FILE_ORDER_REPOSITORY_H

#include <string>
#include <unordered_map>

#include "../Domain/OrderRepository.h"

class FileOrderRepository : public IOrderRepository {
private:
    std::string filePath_;
    std::unordered_map<std::string, Order> cache_;

public:
    explicit FileOrderRepository(const std::string& filePath)
        : filePath_(filePath) {
        loadFromFile();
    }

    ~FileOrderRepository() {
        saveToFile();
    }

    void save(const Order& order) override;

    const Order& getById(const std::string& id) const override;

    std::vector<Order> getAll() const override;

    void update(const Order& order) override;

    void deleteById(const std::string& id) override;

    void saveToFile();
    
    void loadFromFile();
};

#endif
