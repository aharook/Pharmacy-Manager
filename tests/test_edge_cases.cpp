#include <gtest/gtest.h>
#include <filesystem>
#include <string>

#include "Application/InventoryService.h"
#include "Infrastructure/FileProductRepository.h"
#include "Domain/Product.h"

namespace {
std::string tempFilePath(const std::string& name) {
    return "test_edge_" + name + ".txt";
}

void removeFile(const std::string& path) {
    std::error_code ec;
    std::filesystem::remove(path, ec);
}
}

TEST(ProductEdgeTests, DecreaseStockToZero) {
    Product product("Vitamin C", 5, 3.5);

    product.decreasePackCount(5);
    EXPECT_EQ(product.getPackCount(), 0);
}

TEST(InventoryServiceEdgeTests, AddProductZeroQuantityAndPrice) {
    const std::string productsFile = tempFilePath("products_zero");
    removeFile(productsFile);

    {
        FileProductRepository repo(productsFile);
        InventoryService service(repo);
        EXPECT_NO_THROW(service.addProduct("FreeSample", 0, 0.0));

        const auto& products = service.getAllProducts();
        ASSERT_EQ(products.size(), 1u);
        EXPECT_EQ(products[0].getName(), "FreeSample");
        EXPECT_EQ(products[0].getPackCount(), 0);
        EXPECT_DOUBLE_EQ(products[0].getPrice(), 0.0);
    }

    removeFile(productsFile);
}
