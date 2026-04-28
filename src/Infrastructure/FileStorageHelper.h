#ifndef INFRASTRUCTURE_FILE_STORAGE_HELPER_H
#define INFRASTRUCTURE_FILE_STORAGE_HELPER_H

#include <chrono>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

// Simple JSON helper for serialization
class JsonBuilder {
private:
    std::string json_;
    bool isArray_;

public:
    explicit JsonBuilder(bool isArray = false) : isArray_(isArray) {
        json_ = isArray ? "[" : "{";
    }

    JsonBuilder& addString(const std::string& key, const std::string& value) {
        if (json_.size() > 1) json_ += ",";
        json_ += "\"" + key + "\":\"" + escapeJson(value) + "\"";
        return *this;
    }

    JsonBuilder& addNumber(const std::string& key, double value) {
        if (json_.size() > 1) json_ += ",";
        json_ += "\"" + key + "\":" + std::to_string(value);
        return *this;
    }

    JsonBuilder& addInt(const std::string& key, int value) {
        if (json_.size() > 1) json_ += ",";
        json_ += "\"" + key + "\":" + std::to_string(value);
        return *this;
    }

    JsonBuilder& addBool(const std::string& key, bool value) {
        if (json_.size() > 1) json_ += ",";
        json_ += "\"" + key + "\":" + (value ? "true" : "false");
        return *this;
    }

    JsonBuilder& addNumber(const std::string& key, long long value) {
        if (json_.size() > 1) json_ += ",";
        json_ += "\"" + key + "\":" + std::to_string(value);
        return *this;
    }

    JsonBuilder& addElement(const JsonBuilder& element) {
        if (json_.size() > 1) json_ += ",";
        json_ += element.build();
        return *this;
    }

    std::string build() const {
        return json_ + (isArray_ ? "]" : "}");
    }

private:
    static std::string escapeJson(const std::string& input) {
        std::string output;
        for (char c : input) {
            switch (c) {
                case '"': output += "\\\""; break;
                case '\\': output += "\\\\"; break;
                case '\n': output += "\\n"; break;
                case '\r': output += "\\r"; break;
                case '\t': output += "\\t"; break;
                default: output += c;
            }
        }
        return output;
    }
};

class JsonValue {
public:
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    long long getLongLong(const std::string& key, long long defaultValue = 0) const;
    
    static JsonValue parse(const std::string& json);
    static std::vector<JsonValue> parseArray(const std::string& json);

    std::string toJson() const;

private:
    std::string raw_;
};

class FileStorageHelper {
public:
    static std::string readFile(const std::string& filePath);
    static void writeFile(const std::string& filePath, const std::string& content);
    static bool fileExists(const std::string& filePath);
    static void createBackup(const std::string& filePath);
};

#endif
