#include <iostream>
#include "Console/PharmacyConsole.h"
#include "Infrastructure/FileOrderRepository.h"
#include "Infrastructure/FileProductRepository.h"

int main() {
    try {
        FileOrderRepository orderRepository("pharmacy_orders.txt");
        FileProductRepository productRepository("pharmacy_products.txt");
        PharmacyConsole console(orderRepository, productRepository);
        console.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
