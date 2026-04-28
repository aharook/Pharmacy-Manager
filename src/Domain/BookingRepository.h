#ifndef DOMAIN_BOOKING_REPOSITORY_H
#define DOMAIN_BOOKING_REPOSITORY_H

#include <optional>
#include <string>
#include <vector>

#include "Booking.h"

class IBookingRepository {
public:
    virtual ~IBookingRepository() = default;

    virtual void save(const Booking& booking) = 0;
    virtual std::optional<Booking> getById(const std::string& id) const = 0;
    virtual std::vector<Booking> getByProductName(const std::string& productName) const = 0;
    virtual std::vector<Booking> getByCustomerId(const std::string& customerId) const = 0;
    virtual std::vector<Booking> getActive() const = 0;
    virtual void update(const Booking& booking) = 0;
    virtual void deleteById(const std::string& id) = 0;
    virtual std::vector<Booking> getAll() const = 0;
};

#endif
