#include "ShoppingCart.h"
#include <iomanip>
#include <iostream>

bool ShoppingCart::addProduct(const Catalog& catalog, int productId, int quantity) {
    if (quantity <= 0 || !catalog.productExists(productId)) return false;
    std::lock_guard<std::mutex> lock(mutex_);
    quantities_[productId] += quantity;
    return true;
}

bool ShoppingCart::removeProduct(int productId) {
    std::lock_guard<std::mutex> lock(mutex_);
    return quantities_.erase(productId) > 0;
}

bool ShoppingCart::updateQuantity(const Catalog& catalog, int productId, int quantity) {
    if (!catalog.productExists(productId)) return false;
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = quantities_.find(productId);
    if (it == quantities_.end()) return false;
    if (quantity <= 0) quantities_.erase(it);
    else it->second = quantity;
    return true;
}

std::vector<CartLine> ShoppingCart::getItems(const Catalog& catalog) const {
    std::vector<CartLine> lines;
    std::lock_guard<std::mutex> lock(mutex_);
    for (const auto& entry : quantities_) {
        CartLine line{};
        if (catalog.getProductInfo(entry.first, line)) {
            line.quantity = entry.second;
            lines.push_back(line);
        }
    }
    return lines;
}

double ShoppingCart::calculateTotal(const Catalog& catalog) const {
    double total = 0.0;
    for (const auto& item : getItems(catalog)) total += item.unitPrice * item.quantity;
    return total;
}

bool ShoppingCart::isEmpty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return quantities_.empty();
}

void ShoppingCart::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    quantities_.clear();
}

void ShoppingCart::display(const Catalog& catalog) const {
    const auto items = getItems(catalog);
    if (items.empty()) {
        std::cout << "Your cart is empty.\n";
        return;
    }
    double total = 0.0;
    std::cout << "\n" << std::left << std::setw(24) << "Product" << std::setw(8) << "Qty"
              << std::setw(12) << "Unit price" << "Subtotal\n";
    for (const auto& item : items) {
        const double subtotal = item.unitPrice * item.quantity;
        total += subtotal;
        std::cout << std::left << std::setw(24) << item.name << std::setw(8) << item.quantity
                  << "$" << std::setw(11) << std::fixed << std::setprecision(2) << item.unitPrice
                  << "$" << std::fixed << std::setprecision(2) << subtotal << '\n';
    }
    std::cout << "Cart total: $" << std::fixed << std::setprecision(2) << total << '\n';
}
