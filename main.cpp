#include "Catalog.h"
#include "User.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace {
std::mutex outputMutex;

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Please enter a valid whole number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double readDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 0.0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cout << "Please enter a valid non-negative amount.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

std::unique_ptr<User> createUser() {
    std::string name = readLine("Name: ");
    std::string email = readLine("Email: ");
    std::string address = readLine("Shipping address: ");
    std::cout << "Membership type: 1) Regular  2) Premium (10% off)  3) VIP (20% off)\n";
    int choice = readInt("Choose membership: ");
    if (choice == 2) return std::make_unique<PremiumUser>(name, email, address);
    if (choice == 3) return std::make_unique<VIPUser>(name, email, address);
    if (choice != 1) std::cout << "Invalid choice; creating a Regular user.\n";
    return std::make_unique<RegularUser>(name, email, address);
}

void printOrder(const User& user, const std::vector<CartLine>& items, double subtotal, double discount, double total) {
    std::cout << "\n========== ORDER RECEIPT ==========\n"
              << "Customer: " << user.getName() << " (" << user.getMembershipType() << ")\n"
              << "Email: " << user.getEmail() << "\n"
              << "Shipping to: " << user.getShippingAddress() << "\n";
    for (const auto& item : items) {
        std::cout << "- " << item.name << " x " << item.quantity << " @ $"
                  << std::fixed << std::setprecision(2) << item.unitPrice << " = $"
                  << item.unitPrice * item.quantity << '\n';
    }
    std::cout << "Subtotal: $" << std::fixed << std::setprecision(2) << subtotal << '\n'
              << "Discount: -$" << discount << '\n'
              << "Total paid (simulated): $" << total << '\n'
              << "===================================\n";
}

void checkout(User& user, Catalog& catalog) {
    const auto items = user.getCart().getItems(catalog);
    if (items.empty()) {
        std::cout << "Your cart is empty or contains products no longer in the catalog.\n";
        return;
    }
    double subtotal = 0.0;
    for (const auto& item : items) subtotal += item.unitPrice * item.quantity;
    const double discount = subtotal * user.getDiscountRate();
    const double total = subtotal - discount;

    std::cout << "Subtotal: $" << std::fixed << std::setprecision(2) << subtotal
              << " | Membership discount: " << user.getDiscountRate() * 100.0 << "%"
              << " | Total: $" << total << '\n';
    std::string confirmation = readLine("Complete this simulated purchase? (yes/no): ");
    if (confirmation != "yes" && confirmation != "YES" && confirmation != "Yes") {
        std::cout << "Checkout cancelled. Your cart has been kept.\n";
        return;
    }

    std::string error;
    if (!catalog.checkout(items, error)) {
        std::cout << "Checkout failed: " << error << '\n';
        return;
    }
    printOrder(user, items, subtotal, discount, total);
    user.getCart().clear();
}

void simulateConcurrentShopping(Catalog& catalog, int demoProductId) {
    std::cout << "\nRunning concurrency demonstration: two users try to purchase the last unit.\n";
    if (demoProductId <= 0) {
        std::cout << "Could not create the temporary concurrency test item.\n";
        return;
    }
    std::vector<CartLine> orderA{{demoProductId, "Demo product", 1.0, 1}};
    std::vector<CartLine> orderB{{demoProductId, "Demo product", 1.0, 1}};
    bool successA = false;
    bool successB = false;
    std::string errorA, errorB;

    std::thread first([&] {
        successA = catalog.checkout(orderA, errorA);
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "Concurrent user A: " << (successA ? "purchase succeeded" : "purchase rejected - " + errorA) << '\n';
    });
    std::thread second([&] {
        successB = catalog.checkout(orderB, errorB);
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "Concurrent user B: " << (successB ? "purchase succeeded" : "purchase rejected - " + errorB) << '\n';
    });
    first.join();
    second.join();
    std::cout << "Expected result: exactly one purchase succeeds because only one unit was available.\n";
    catalog.removeProduct(demoProductId); // Remove the temporary item after the test.
}

void seedCatalog(Catalog& catalog) {
    catalog.addProduct("Wireless Mouse", "Ergonomic wireless mouse", 24.99, 12);
    catalog.addProduct("Mechanical Keyboard", "Compact mechanical keyboard", 69.50, 8);
    catalog.addProduct("USB-C Hub", "Seven-port USB-C hub", 39.95, 15);
    catalog.addProduct("Laptop Stand", "Adjustable aluminum stand", 32.00, 10);
}

void addProductToCatalog(Catalog& catalog) {
    std::string name = readLine("Product name: ");
    std::string description = readLine("Description: ");
    double price = readDouble("Price: $");
    int stock = readInt("Starting stock (0 or more): ");
    if (name.empty() || stock < 0) {
        std::cout << "Name cannot be empty and stock cannot be negative. Product not added.\n";
        return;
    }
    int id = catalog.addProduct(name, description, price, stock);
    if (id < 0) std::cout << "Could not add product. Check the entered values.\n";
    else std::cout << "Product added with ID " << id << ".\n";
}
}

int main() {
    Catalog catalog;
    seedCatalog(catalog);
    std::cout << "====================================\n"
              << "       ONLINE SHOPPING SYSTEM        \n"
              << "====================================\n";
    auto user = createUser();
    std::cout << "Welcome, " << user->getName() << "! Membership: " << user->getMembershipType()
              << " (" << user->getDiscountRate() * 100.0 << "% discount)\n";

    bool running = true;
    while (running) {
        std::cout << "\nMenu\n"
                  << "1. Browse catalog\n"
                  << "2. Add product to cart\n"
                  << "3. Remove product from cart\n"
                  << "4. Update cart quantity\n"
                  << "5. View shopping cart\n"
                  << "6. Checkout\n"
                  << "7. Add a product to the catalog\n"
                  << "8. Remove a product from the catalog\n"
                  << "9. Update a catalog product\n"
                  << "10. Run multithreading demonstration\n"
                  << "11. Exit\n";
        int choice = readInt("Select an option: ");
        switch (choice) {
            case 1:
                catalog.displayProducts();
                break;
            case 2: {
                catalog.displayProducts();
                int id = readInt("Enter product ID: ");
                int quantity = readInt("Quantity to add: ");
                if (quantity <= 0) std::cout << "Quantity must be greater than zero.\n";
                else if (!catalog.productExists(id)) std::cout << "That product ID does not exist.\n";
                else if (user->getCart().addProduct(catalog, id, quantity)) std::cout << "Added to cart. Stock is verified again during checkout.\n";
                else std::cout << "Could not add that product.\n";
                break;
            }
            case 3: {
                user->getCart().display(catalog);
                int id = readInt("Product ID to remove: ");
                std::cout << (user->getCart().removeProduct(id) ? "Item removed.\n" : "Item was not in your cart.\n");
                break;
            }
            case 4: {
                user->getCart().display(catalog);
                int id = readInt("Product ID to update: ");
                int quantity = readInt("New quantity (0 removes item): ");
                std::cout << (user->getCart().updateQuantity(catalog, id, quantity) ? "Cart quantity updated.\n" : "Could not update that cart item.\n");
                break;
            }
            case 5:
                user->getCart().display(catalog);
                break;
            case 6:
                checkout(*user, catalog);
                break;
            case 7:
                addProductToCatalog(catalog);
                break;
            case 8: {
                catalog.displayProducts();
                int id = readInt("Product ID to remove from catalog: ");
                std::cout << (catalog.removeProduct(id) ? "Product removed from catalog.\n" : "Product ID not found.\n");
                break;
            }
            case 9: {
                catalog.displayProducts();
                int id = readInt("Product ID to update: ");
                std::string name = readLine("New product name: ");
                std::string description = readLine("New description: ");
                double price = readDouble("New price: $");
                if (name.empty()) std::cout << "Product name cannot be empty.\n";
                else std::cout << (catalog.updateProduct(id, name, description, price) ? "Product updated.\n" : "Could not update product; check the ID and values.\n");
                break;
            }
            case 10: {
                int demoId = catalog.addProduct("Concurrency Test Item", "Temporary item for thread-safety demo", 1.00, 1);
                simulateConcurrentShopping(catalog, demoId);
                break;
            }
            case 11:
                running = false;
                std::cout << "Thanks for visiting the Online Shopping System!\n";
                break;
            default:
                std::cout << "Please select a menu option from 1 to 11.\n";
        }
    }
    return 0;
}
