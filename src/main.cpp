#include <iostream>
#include "Console/PharmacyConsole.h"
#include "Infrastructure/FileOrderRepository.h"

int main() {
    try {
        FileOrderRepository orderRepository("pharmacy_orders.txt");
        PharmacyConsole console(orderRepository);
        console.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
