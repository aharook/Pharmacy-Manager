#ifndef INFRASTRUCTURE_PRODUCT_SERIALIZER_H
#define INFRASTRUCTURE_PRODUCT_SERIALIZER_H

#include <string>
#include "../Domain/Product.h"

class ProductSerializer {
public:
    std::string serialize(const Product& product) const;
};

#endif
