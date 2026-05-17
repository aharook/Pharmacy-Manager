#ifndef DOMAIN_PRODUCT_REPOSITORY_H
#define DOMAIN_PRODUCT_REPOSITORY_H

#include <string>
#include <vector>
#include "Product.h"

class IProductRepository {
public:
    virtual ~IProductRepository() = default;

    virtual void save(const Product& product) = 0;
    virtual const std::vector<Product>& getAll() const = 0;
    virtual Product* findByName(const std::string& name) = 0;
    virtual const Product* findByName(const std::string& name) const = 0;
};

#endif
