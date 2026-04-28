#ifndef DOMAIN_BOOKING_STATUS_H
#define DOMAIN_BOOKING_STATUS_H

#include <string>

enum class BookingStatus {
    RESERVED,     // Товар забронений, але не видан
    CONFIRMED,    // Бронювання підтверджено
    COMPLETED,    // Бронювання завершено (товар видан)
    CANCELLED     // Бронювання скасовано (товар повернено в склад)
};

inline const char* bookingStatusToString(BookingStatus status) {
    switch (status) {
        case BookingStatus::RESERVED: return "RESERVED";
        case BookingStatus::CONFIRMED: return "CONFIRMED";
        case BookingStatus::COMPLETED: return "COMPLETED";
        case BookingStatus::CANCELLED: return "CANCELLED";
        default: return "UNKNOWN";
    }
}

inline BookingStatus stringToBookingStatus(const std::string& str) {
    if (str == "CONFIRMED") return BookingStatus::CONFIRMED;
    if (str == "COMPLETED") return BookingStatus::COMPLETED;
    if (str == "CANCELLED") return BookingStatus::CANCELLED;
    return BookingStatus::RESERVED;
}

#endif
