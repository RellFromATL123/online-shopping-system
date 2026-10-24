#include "Catalog.h"
#include <iomanip>
#include <iostream>

Catalog::Catalog() : nextId_(1) {}

int Catalog::addProduct(const std::string& name, const std::string& description, double price, int stock) {
    if (name.empty() || price < 0.0 || stock < 0) return -1;
    std::lock_guard<std::mutex> lock(mutex_);
    const int id = nextId_++;
    products_.emplace(id, Product(id, name, description, price, stock));
    return id;
}

bool Catalog::removeProduct(int productId) {
    std::lock_guard<std::mutex> lock(mutex_);
    return products_.erase(productId) > 0;
}

bool Catalog::updateProduct(int productId, const std::string& name, const std::string& description, double price) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = products_.find(productId);
    return it != products_.end() && it->second.updateDetails(name, description, price);
}

bool Catalog::updateStock(int productId, int quantity) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = products_.find(productId);
    return it != products_.end() && it->second.setStockQuantity(quantity);
}

void Catalog::displayProducts() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::cout << "\nID   " << std::left << std::setw(24) << "Product"
              << std::setw(10) << "Price" << std::setw(8) << "Stock" << "Description\n";
    std::cout << std::string(75, '-') << '\n';
    for (const auto& entry : products_) entry.second.display();
}

bool Catalog::productExists(int productId) const {
    std::lock_guard<std::mutex> lock(mutex_);
    return products_.find(productId) != products_.end();
}

bool Catalog::hasStock(int productId, int quantity) const {
    if (quantity <= 0) return false;
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = products_.find(productId);
    return it != products_.end() && it->second.getStockQuantity() >= quantity;
}

bool Catalog::getProductInfo(int productId, CartLine& result) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = products_.find(productId);
    if (it == products_.end()) return false;
    result = {productId, it->second.getName(), it->second.getPrice(), 1};
    return true;
}

bool Catalog::checkout(const std::vector<CartLine>& items, std::string& errorMessage) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (items.empty()) {
        errorMessage = "Your cart is empty.";
        return false;
    }

    // Validate every line before changing any stock, so a failed order is not partially processed.
    for (const auto& line : items) {
        auto it = products_.find(line.productId);
        if (it == products_.end()) {
            errorMessage = "A product in the cart is no longer in the catalog.";
            return false;
        }
        if (line.quantity <= 0 || it->second.getStockQuantity() < line.quantity) {
            errorMessage = "Not enough stock for " + it->second.getName() + ". No items were purchased.";
            return false;
        }
    }

    for (const auto& line : items) {
        products_.at(line.productId).reduceStock(line.quantity);
    }
    return true;
}
