#ifndef USER_H
#define USER_H

#include "ShoppingCart.h"
#include <string>

class User {
private:
    std::string name_;
    std::string email_;
    std::string shippingAddress_;
    ShoppingCart cart_;

protected:
    User(const std::string& name, const std::string& email, const std::string& shippingAddress);

public:
    virtual ~User() = default;
    User(const User&) = delete;
    User& operator=(const User&) = delete;

    std::string getName() const;
    std::string getEmail() const;
    std::string getShippingAddress() const;
    ShoppingCart& getCart();
    const ShoppingCart& getCart() const;
    virtual std::string getMembershipType() const = 0;
    virtual double getDiscountRate() const = 0;
};

class RegularUser : public User {
public:
    RegularUser(const std::string& name, const std::string& email, const std::string& shippingAddress);
    std::string getMembershipType() const override;
    double getDiscountRate() const override;
};

class PremiumUser : public User {
public:
    PremiumUser(const std::string& name, const std::string& email, const std::string& shippingAddress);
    std::string getMembershipType() const override;
    double getDiscountRate() const override;
};

class VIPUser : public User {
public:
    VIPUser(const std::string& name, const std::string& email, const std::string& shippingAddress);
    std::string getMembershipType() const override;
    double getDiscountRate() const override;
};

#endif
