#ifndef DOMAIN_RETURN_REPOSITORY_H
#define DOMAIN_RETURN_REPOSITORY_H

#include <optional>
#include <string>
#include <vector>

#include "Return.h"

class IReturnRepository {
public:
    virtual ~IReturnRepository() = default;

    virtual void save(const Return& ret) = 0;
    virtual std::optional<Return> getById(const std::string& id) const = 0;
    virtual std::vector<Return> getByOrderId(const std::string& orderId) const = 0;
    virtual std::vector<Return> getAll() const = 0;
    virtual std::vector<Return> getOverdueReturns() const = 0;
};

#endif
