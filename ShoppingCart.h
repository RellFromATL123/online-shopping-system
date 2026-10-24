#ifndef SHOPPING_CART_H
#define SHOPPING_CART_H

#include "Catalog.h"
#include <map>
#include <mutex>
#include <vector>

class ShoppingCart {
private:
    std::map<int, int> quantities_;
    mutable std::mutex mutex_;

public:
    ShoppingCart() = default;
    ShoppingCart(const ShoppingCart&) = delete;
    ShoppingCart& operator=(const ShoppingCart&) = delete;

    bool addProduct(const Catalog& catalog, int productId, int quantity);
    bool removeProduct(int productId);
    bool updateQuantity(const Catalog& catalog, int productId, int quantity);
    std::vector<CartLine> getItems(const Catalog& catalog) const;
    double calculateTotal(const Catalog& catalog) const;
    bool isEmpty() const;
    void clear();
    void display(const Catalog& catalog) const;
};

#endif
