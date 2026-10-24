#ifndef CATALOG_H
#define CATALOG_H

#include "Product.h"
#include <map>
#include <mutex>
#include <string>
#include <vector>

struct CartLine {
    int productId;
    std::string name;
    double unitPrice;
    int quantity;
};

class Catalog {
private:
    std::map<int, Product> products_;
    mutable std::mutex mutex_;
    int nextId_;

public:
    Catalog();
    Catalog(const Catalog&) = delete;
    Catalog& operator=(const Catalog&) = delete;

    int addProduct(const std::string& name, const std::string& description, double price, int stock);
    bool removeProduct(int productId);
    bool updateProduct(int productId, const std::string& name, const std::string& description, double price);
    bool updateStock(int productId, int quantity);
    void displayProducts() const;
    bool productExists(int productId) const;
    bool hasStock(int productId, int quantity) const;
    bool getProductInfo(int productId, CartLine& result) const;
    bool checkout(const std::vector<CartLine>& items, std::string& errorMessage);
};

#endif
