#include "FileBookingRepository.h"

#include <chrono>

#include "FileStorageHelper.h"

void FileBookingRepository::save(const Booking& booking) {
    cache_[booking.getId()] = booking;
    isDirty_ = true;
    saveToFile();
}

std::optional<Booking> FileBookingRepository::getById(const std::string& id) const {
    auto it = cache_.find(id);
    if (it == cache_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::vector<Booking> FileBookingRepository::getByProductName(
    const std::string& productName) const {
    std::vector<Booking> result;
    for (const auto& [id, booking] : cache_) {
        if (booking.getProductName() == productName) {
            result.push_back(booking);
        }
    }
    return result;
}

std::vector<Booking> FileBookingRepository::getByCustomerId(
    const std::string& customerId) const {
    std::vector<Booking> result;
    for (const auto& [id, booking] : cache_) {
        if (booking.getCustomerId() == customerId) {
            result.push_back(booking);
        }
    }
    return result;
}

std::vector<Booking> FileBookingRepository::getActive() const {
    std::vector<Booking> result;
    for (const auto& [id, booking] : cache_) {
        if (booking.isActive() && !booking.isExpired()) {
            result.push_back(booking);
        }
    }
    return result;
}

void FileBookingRepository::update(const Booking& booking) {
    cache_[booking.getId()] = booking;
    isDirty_ = true;
    saveToFile();
}

void FileBookingRepository::deleteById(const std::string& id) {
    if (cache_.erase(id) > 0) {
        isDirty_ = true;
        saveToFile();
    }
}

std::vector<Booking> FileBookingRepository::getAll() const {
    std::vector<Booking> result;
    for (const auto& [id, booking] : cache_) {
        result.push_back(booking);
    }
    return result;
}

void FileBookingRepository::saveToFile() {
    JsonBuilder arrayBuilder(true);
    
    for (const auto& [id, booking] : cache_) {
        arrayBuilder.addElement(JsonBuilder()
            .addString("id", booking.getId())
            .addString("productName", booking.getProductName())
            .addInt("quantity", booking.getQuantity())
            .addString("customerId", booking.getCustomerId())
            .addString("status", bookingStatusToString(booking.getStatus()))
            .addNumber("createdAt", static_cast<long long>(
                booking.getCreatedAt().time_since_epoch().count()))
            .addNumber("expiresAt", static_cast<long long>(
                booking.getExpiresAt().time_since_epoch().count())));
    }

    std::string json = arrayBuilder.build();
    
    try {
        FileStorageHelper::writeFile(filePath_, json);
        isDirty_ = false;
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to save bookings to file: " + std::string(e.what()));
    }
}

void FileBookingRepository::loadFromFile() {
    cache_.clear();
    
    if (!FileStorageHelper::fileExists(filePath_)) {
        return;
    }

    try {
        std::string content = FileStorageHelper::readFile(filePath_);
        
        if (content.empty()) {
            return;
        }

        auto items = JsonValue::parseArray(content);
        for (const auto& item : items) {
            std::string id = item.getString("id");
            std::string productName = item.getString("productName");
            int quantity = item.getInt("quantity", 0);
            std::string customerId = item.getString("customerId");
            BookingStatus status = stringToBookingStatus(item.getString("status"));
            long long createdTimestamp = item.getLongLong("createdAt", 0);
            long long expiresTimestamp = item.getLongLong("expiresAt", 0);
            
            auto createdAt = std::chrono::system_clock::time_point(
                std::chrono::system_clock::duration(createdTimestamp));
            auto expiresAt = std::chrono::system_clock::time_point(
                std::chrono::system_clock::duration(expiresTimestamp));

            if (!id.empty() && !productName.empty()) {
                Booking booking(id, productName, quantity, customerId, status, createdAt, expiresAt);
                cache_[id] = booking;
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load bookings from file: " + std::string(e.what()));
    }
}
