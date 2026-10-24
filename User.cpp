#include "User.h"
#include <utility>

User::User(const std::string& name, const std::string& email, const std::string& shippingAddress)
    : name_(name), email_(email), shippingAddress_(shippingAddress) {}

std::string User::getName() const { return name_; }
std::string User::getEmail() const { return email_; }
std::string User::getShippingAddress() const { return shippingAddress_; }
ShoppingCart& User::getCart() { return cart_; }
const ShoppingCart& User::getCart() const { return cart_; }

RegularUser::RegularUser(const std::string& name, const std::string& email, const std::string& shippingAddress)
    : User(name, email, shippingAddress) {}

PremiumUser::PremiumUser(const std::string& name, const std::string& email, const std::string& shippingAddress)
    : User(name, email, shippingAddress) {}

VIPUser::VIPUser(const std::string& name, const std::string& email, const std::string& shippingAddress)
    : User(name, email, shippingAddress) {}

std::string RegularUser::getMembershipType() const { return "Regular"; }
double RegularUser::getDiscountRate() const { return 0.0; }
std::string PremiumUser::getMembershipType() const { return "Premium"; }
double PremiumUser::getDiscountRate() const { return 0.10; }
std::string VIPUser::getMembershipType() const { return "VIP"; }
double VIPUser::getDiscountRate() const { return 0.20; }
