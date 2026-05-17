#ifndef CALCULATIONS_BOOKING_H
#define CALCULATIONS_BOOKING_H

#include <string>

class BookingCalculation {
private:
    static constexpr double PENALTY_RATE = 0.20;

public:

    static double calculatePenalty(double orderTotal, bool isMissed) {
        if (!isMissed) {
            return orderTotal;
        }
        return orderTotal * (1.0 - PENALTY_RATE);
    }

    static constexpr double getPenaltyRate() {
        return PENALTY_RATE;
    }
};

#endif
