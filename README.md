# Online Shopping System

#### Video Demo: <Add your unlisted video URL here after recording>

#### Description

The Online Shopping System is a console-based C++ application that demonstrates object-oriented programming, inheritance, encapsulation, input validation, simulated checkout, and multithreading. A customer can browse a product catalog, add products to a shopping cart, remove products, update quantities, review item prices and totals, and complete a simulated purchase. The application does not process real payments or collect real payment-card information. It is an educational demonstration of the main workflow in an online store.

When the program starts, it asks for the customer's name, email, shipping address, and membership type. The system supports Regular, Premium, and VIP users. Regular users pay the regular total, Premium users receive a 10% discount, and VIP users receive a 20% discount. These membership rules are implemented through inheritance and virtual functions so that the checkout logic can work with a base `User` reference while using the selected user's discount rate.

The catalog begins with several sample products, each with an ID, name, description, price, and stock quantity. Users can display the catalog and add new products during the session. The `Catalog` class also contains methods to remove a product, update product details, and change stock quantity. Catalog methods validate basic values such as non-negative prices and stock quantities. Product IDs are assigned by the catalog so each product can be identified consistently during a session.

The shopping cart stores product IDs and quantities rather than making separate copies of complete products. This design avoids treating a cart's copy of a product as the authoritative inventory record. Product names and current prices are looked up in the catalog when the cart is displayed or totaled. Users can add an item, remove an item, or change its quantity. Entering zero as an updated quantity removes the item. Invalid product IDs and non-positive quantities are rejected where appropriate.

Checkout is simulated in the console. The customer sees the subtotal, membership discount, and final total, then confirms the purchase by entering `yes`. Before reducing stock, the catalog checks every line in the order. If a product has been removed or the requested quantity is no longer available, the order is rejected and no stock is changed. This validate-first approach avoids partially processing an order. If checkout succeeds, the program prints an order receipt and clears the cart. The stock quantity is then lower by the amount purchased.

Multithreading is demonstrated with two `std::thread` tasks that attempt to buy the last unit of the same product at nearly the same time. The catalog uses a `std::mutex` and `std::lock_guard` to protect shared product data and stock changes. The complete stock validation and stock-reduction operation takes place while holding the catalog mutex, so one thread cannot change inventory between another thread's validation and update. With one unit available and two one-unit orders, only one order should succeed. A separate mutex protects the demonstration's console messages from overlapping. The demo uses a temporary test product and removes it afterward so the regular catalog remains unchanged.

## Project files

- `main.cpp` contains the program entry point, interactive menu, input helpers, sample catalog setup, checkout workflow, and multithreading demonstration.
- `Product.h` declares the `Product` class and its public interface.
- `Product.cpp` implements product data access, detail updates, stock validation, stock reduction, and catalog display formatting.
- `Catalog.h` declares the `Catalog` class and the `CartLine` structure used to represent an item in an order.
- `Catalog.cpp` implements catalog operations and thread-safe inventory access using a mutex.
- `ShoppingCart.h` declares the `ShoppingCart` class and its item-management functions.
- `ShoppingCart.cpp` implements adding, removing, updating, displaying, and totaling cart items.
- `User.h` declares the abstract `User` base class and the `RegularUser`, `PremiumUser`, and `VIPUser` derived classes.
- `User.cpp` implements user information access and membership-specific discount behavior.
- `README.md` documents the application's purpose, structure, design decisions, and video demonstration link.

## Design decisions

The application separates product data, catalog inventory, cart contents, and customer membership into different classes. This supports single responsibility and makes the code easier to maintain than placing the entire application in `main.cpp`. Private data members protect the internal state of each class, while public methods provide controlled access. Inheritance and virtual functions demonstrate runtime polymorphism for membership discounts. The catalog owns the authoritative stock values, and a mutex protects shared inventory during concurrent checkout. `std::lock_guard` releases the mutex automatically when a protected operation ends, including when a function exits early. Input helper functions repeatedly prompt when a numeric value is invalid, and checkout validates all items before changing inventory.

## Build and run

A C++17-compatible compiler is recommended. From the project directory, run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp Product.cpp Catalog.cpp ShoppingCart.cpp User.cpp -pthread -o shopping_system
./shopping_system
```

On Windows, run the compiled executable as `shopping_system.exe` from the terminal. The `-pthread` flag enables the standard threading support required by this project on common GCC/Linux and macOS setups.

## Testing checklist

1. Browse the catalog and confirm that sample products and stock quantities appear.
2. Add a valid product to the cart, view the cart, update a quantity, and remove an item.
3. Try an invalid product ID, a negative quantity, and invalid menu input.
4. Create Regular, Premium, and VIP users in separate runs and compare their discounts at checkout.
5. Complete a purchase and browse the catalog again to confirm stock decreased.
6. Try purchasing more units than are in stock and confirm checkout rejects the order without changing any inventory.
7. Run the multithreading demonstration and confirm exactly one of the two competing orders succeeds.

## Video demonstration

Before submitting, replace the placeholder URL at the top with the URL of your uploaded, non-private video. Keep the video to five minutes or less and begin with a title slide that includes the project title, your name, GitHub username, city and country, and the recording date. Then demonstrate the catalog, cart operations, membership discount, checkout and stock update, and concurrent checkout behavior.
