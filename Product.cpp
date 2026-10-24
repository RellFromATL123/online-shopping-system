#include "Product.h"
#include <iomanip>
#include <iostream>
#include <utility>

Product::Product(int id, const std::string& name, const std::string& description,
                 double price, int stockQuantity)
    : id_(id), name_(name), description_(description), price_(price), stockQuantity_(stockQuantity) {}

int Product::getId() const { return id_; }
std::string Product::getName() const { return name_; }
std::string Product::getDescription() const { return description_; }
double Product::getPrice() const { return price_; }
int Product::getStockQuantity() const { return stockQuantity_; }

bool Product::updateDetails(const std::string& name, const std::string& description, double price) {
    if (name.empty() || price < 0.0) return false;
    name_ = name;
    description_ = description;
    price_ = price;
    return true;
}

bool Product::setStockQuantity(int quantity) {
    if (quantity < 0) return false;
    stockQuantity_ = quantity;
    return true;
}

bool Product::reduceStock(int quantity) {
    if (quantity <= 0 || quantity > stockQuantity_) return false;
    stockQuantity_ -= quantity;
    return true;
}

void Product::display() const {
    std::cout << std::left << std::setw(5) << id_
              << std::setw(24) << name_
              << std::setw(10) << std::fixed << std::setprecision(2) << price_
              << std::setw(8) << stockQuantity_
              << description_ << '\n';
}
