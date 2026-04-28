#ifndef APPLICATION_ORDER_DTOS_H
#define APPLICATION_ORDER_DTOS_H

#include <string>



struct OrderRequestItem {
    std::string productName;
    int quantity = 0;
};



#endif
