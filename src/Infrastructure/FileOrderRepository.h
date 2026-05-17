#ifndef INFRASTRUCTURE_FILE_ORDER_REPOSITORY_H
#define INFRASTRUCTURE_FILE_ORDER_REPOSITORY_H

#include <string>
#include <unordered_map>
#include <memory>

#include "../Domain/OrderRepository.h"
#include "OrderSerializer.h"
#include "OrderDeserializer.h"

class FileOrderRepository : public IOrderRepository {
private:
    std::string filePath_;
    std::unordered_map<std::string, Order> cache_;
    std::unique_ptr<OrderSerializer> serializer_;
    std::unique_ptr<OrderDeserializer> deserializer_;

public:
    explicit FileOrderRepository(const std::string& filePath);

    ~FileOrderRepository();

    void save(const Order& order) override;

    const Order& getById(const std::string& id) const override;

    std::vector<Order> getAll() const override;

    void update(const Order& order) override;

    void deleteById(const std::string& id) override;

private:
    void saveToFile();
    void loadFromFile();
};

#endif
