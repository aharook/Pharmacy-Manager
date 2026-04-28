#include "FileStorageHelper.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <regex>

// Simple JSON parsing helper
static std::string extractValue(const std::string& json, const std::string& key) {
    std::string pattern = "\"" + key + "\"\\s*:\\s*";
    size_t keyPos = json.find("\"" + key + "\"");
    if (keyPos == std::string::npos) {
        return "";
    }

    size_t colonPos = json.find(":", keyPos);
    if (colonPos == std::string::npos) {
        return "";
    }

    size_t startPos = colonPos + 1;
    // Skip whitespace
    while (startPos < json.size() && std::isspace(json[startPos])) {
        startPos++;
    }

    if (startPos >= json.size()) {
        return "";
    }

    // Extract value based on type
    if (json[startPos] == '"') {
        // String value
        size_t endPos = startPos + 1;
        while (endPos < json.size() && json[endPos] != '"') {
            if (json[endPos] == '\\') {
                endPos++; // Skip escaped character
            }
            endPos++;
        }
        if (endPos < json.size()) {
            return json.substr(startPos + 1, endPos - startPos - 1);
        }
    } else if (json[startPos] == '{' || json[startPos] == '[') {
        // Object or array
        int depth = 1;
        size_t endPos = startPos + 1;
        char openChar = json[startPos];
        char closeChar = (openChar == '{') ? '}' : ']';
        
        while (endPos < json.size() && depth > 0) {
            if (json[endPos] == openChar) depth++;
            else if (json[endPos] == closeChar) depth--;
            endPos++;
        }
        return json.substr(startPos, endPos - startPos);
    } else {
        // Number or boolean
        size_t endPos = startPos;
        while (endPos < json.size() && 
               (std::isdigit(json[endPos]) || json[endPos] == '.' || 
                json[endPos] == '-' || json[endPos] == 'e' || json[endPos] == 'E' ||
                json[endPos] == 't' || json[endPos] == 'r' || json[endPos] == 'u' ||
                json[endPos] == 'a' || json[endPos] == 'l' || json[endPos] == 's' ||
                json[endPos] == 'f')) {
            endPos++;
        }
        return json.substr(startPos, endPos - startPos);
    }

    return "";
}

static std::string unescapeJson(const std::string& input) {
    std::string output;
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\\' && i + 1 < input.size()) {
            char next = input[i + 1];
            if (next == 'n') {
                output += '\n';
                ++i;
            } else if (next == 't') {
                output += '\t';
                ++i;
            } else if (next == 'r') {
                output += '\r';
                ++i;
            } else if (next == '"') {
                output += '"';
                ++i;
            } else if (next == '\\') {
                output += '\\';
                ++i;
            } else {
                output += input[i];
            }
        } else {
            output += input[i];
        }
    }
    return output;
}

std::string JsonValue::getString(const std::string& key, const std::string& defaultValue) const {
    std::string value = extractValue(raw_, key);
    if (value.empty()) {
        return defaultValue;
    }
    return unescapeJson(value);
}

int JsonValue::getInt(const std::string& key, int defaultValue) const {
    std::string value = extractValue(raw_, key);
    if (value.empty()) {
        return defaultValue;
    }
    try {
        return std::stoi(value);
    } catch (...) {
        return defaultValue;
    }
}

double JsonValue::getDouble(const std::string& key, double defaultValue) const {
    std::string value = extractValue(raw_, key);
    if (value.empty()) {
        return defaultValue;
    }
    try {
        return std::stod(value);
    } catch (...) {
        return defaultValue;
    }
}

bool JsonValue::getBool(const std::string& key, bool defaultValue) const {
    std::string value = extractValue(raw_, key);
    if (value.empty()) {
        return defaultValue;
    }
    return value == "true" || value == "1";
}

long long JsonValue::getLongLong(const std::string& key, long long defaultValue) const {
    std::string value = extractValue(raw_, key);
    if (value.empty()) {
        return defaultValue;
    }
    try {
        return std::stoll(value);
    } catch (...) {
        return defaultValue;
    }
}

JsonValue JsonValue::parse(const std::string& json) {
    JsonValue value;
    value.raw_ = json;
    return value;
}

std::vector<JsonValue> JsonValue::parseArray(const std::string& json) {
    std::vector<JsonValue> result;
    
    if (json.empty() || json[0] != '[' || json[json.size() - 1] != ']') {
        return result;
    }

    std::string content = json.substr(1, json.size() - 2);
    
    int depth = 0;
    size_t start = 0;
    
    for (size_t i = 0; i < content.size(); ++i) {
        if (content[i] == '{' || content[i] == '[') {
            depth++;
        } else if (content[i] == '}' || content[i] == ']') {
            depth--;
        } else if (content[i] == ',' && depth == 0) {
            std::string item = content.substr(start, i - start);
            // Trim whitespace
            while (!item.empty() && std::isspace(item[0])) item = item.substr(1);
            while (!item.empty() && std::isspace(item.back())) item.pop_back();
            
            if (!item.empty()) {
                result.push_back(JsonValue::parse(item));
            }
            start = i + 1;
        }
    }
    
    if (start < content.size()) {
        std::string item = content.substr(start);
        // Trim whitespace
        while (!item.empty() && std::isspace(item[0])) item = item.substr(1);
        while (!item.empty() && std::isspace(item.back())) item.pop_back();
        
        if (!item.empty()) {
            result.push_back(JsonValue::parse(item));
        }
    }

    return result;
}

std::string JsonValue::toJson() const {
    return raw_;
}

std::string FileStorageHelper::readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filePath);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void FileStorageHelper::writeFile(const std::string& filePath, const std::string& content) {
    // Create backup if file exists
    if (fileExists(filePath)) {
        createBackup(filePath);
    }

    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create/open file: " + filePath);
    }
    file << content;
    file.close();
}

bool FileStorageHelper::fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}

void FileStorageHelper::createBackup(const std::string& filePath) {
    try {
        std::string backupPath = filePath + ".bak";
        std::ifstream src(filePath, std::ios::binary);
        std::ofstream dst(backupPath, std::ios::binary);
        dst << src.rdbuf();
    } catch (...) {
        // Ignore backup errors
    }
}
