#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>

class Product {
private:
    int id_;
    std::string name_;
    std::string description_;
    double price_;
    int stockQuantity_;

public:
    Product(int id, const std::string& name, const std::string& description,
            double price, int stockQuantity);

    int getId() const;
    std::string getName() const;
    std::string getDescription() const;
    double getPrice() const;
    int getStockQuantity() const;

    bool updateDetails(const std::string& name, const std::string& description, double price);
    bool setStockQuantity(int quantity);
    bool reduceStock(int quantity);
    void display() const;
};

#endif
