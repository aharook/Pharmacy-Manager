#ifndef INFRASTRUCTURE_PRODUCT_DESERIALIZER_H
#define INFRASTRUCTURE_PRODUCT_DESERIALIZER_H

#include <string>
#include "../Domain/Product.h"

class ProductDeserializer {
public:
    Product deserialize(const std::string& line) const;
};

#endif
