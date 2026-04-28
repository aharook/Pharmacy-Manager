#ifndef INFRASTRUCTURE_FILE_BOOKING_REPOSITORY_H
#define INFRASTRUCTURE_FILE_BOOKING_REPOSITORY_H

#include <string>
#include <unordered_map>

#include "../Domain/BookingRepository.h"

class FileBookingRepository : public IBookingRepository {
private:
    std::string filePath_;
    std::unordered_map<std::string, Booking> cache_;
    bool isDirty_;

public:
    explicit FileBookingRepository(const std::string& filePath)
        : filePath_(filePath), isDirty_(false) {
        loadFromFile();
    }

    ~FileBookingRepository() {
        if (isDirty_) {
            try {
                saveToFile();
            } catch (...) {
                // Error handling on destruction
            }
        }
    }

    void save(const Booking& booking) override;
    std::optional<Booking> getById(const std::string& id) const override;
    std::vector<Booking> getByProductName(const std::string& productName) const override;
    std::vector<Booking> getByCustomerId(const std::string& customerId) const override;
    std::vector<Booking> getActive() const override;
    void update(const Booking& booking) override;
    void deleteById(const std::string& id) override;
    std::vector<Booking> getAll() const override;

    // Persistence operations
    void saveToFile();
    void loadFromFile();
};

#endif
