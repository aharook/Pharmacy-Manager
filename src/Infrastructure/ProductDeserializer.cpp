#include "ProductDeserializer.h"
#include <sstream>

Product ProductDeserializer::deserialize(const std::string& line) const {
    std::stringstream ss(line);
    std::string name, packCountStr, priceStr;

    if (!std::getline(ss, name, '|') || 
        !std::getline(ss, packCountStr, '|') || 
        !std::getline(ss, priceStr, '|')) {
        throw std::runtime_error("Invalid product format");
    }

    int packCount = std::stoi(packCountStr);
    double price = std::stod(priceStr);

    return Product(name, packCount, price);
}
