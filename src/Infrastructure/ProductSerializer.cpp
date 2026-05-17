#include "ProductSerializer.h"

std::string ProductSerializer::serialize(const Product& product) const {
    std::string result;
    result += product.getName() + "|";
    result += std::to_string(product.getPackCount()) + "|";
    result += std::to_string(product.getPrice());
    return result;
}
