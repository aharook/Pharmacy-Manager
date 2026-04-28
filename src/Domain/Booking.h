#ifndef DOMAIN_BOOKING_H
#define DOMAIN_BOOKING_H

#include <string>

class Booking {
private:
    std::string orderId_;
    bool isMissed_;
    static constexpr double PENALTY_RATE = 0.20;

public:
    Booking() : orderId_(""), isMissed_(false) {}

    Booking(const std::string& orderId, bool isMissed = false)
        : orderId_(orderId), isMissed_(isMissed) {}

    const std::string& getOrderId() const {
        return orderId_;
    }

    bool isMissed() const {
        return isMissed_;
    }

    void markAsMissed() {
        isMissed_ = true;
    }

    double calculatePenalty(double orderTotal) const {
        if (!isMissed_) {
            return orderTotal;
        }
        return orderTotal * (1.0 - PENALTY_RATE);
    }
};

#endif
