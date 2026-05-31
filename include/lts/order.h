#ifndef LTS_ORDER_H
#define LTS_ORDER_H

#include <string>
#include <memory>
#include <iostream>

namespace lts {

#pragma pack(push, 1)
// Fixed-size binary representation matching the Simple Binary Encoding (SBE) stream from Python
struct OrderPacket {
    char action;       // 'B' = Buy, 'S' = Sell
    char padding[3];   // Structured alignment padding for 4-byte boundaries
    char ticker[8];    // Fixed-size 8-byte character array for ticker symbols
    int32_t quantity;  // 4-byte signed integer for trade quantity
    double price;      // 8-byte double-precision floating point for unit price
};
#pragma pack(pop)

// Abstraction & OOP Core: Base class enforcing common data interfaces and contract behaviors
class Order {
protected:
    char m_action;
    std::string m_ticker;
    int32_t m_quantity;
    double m_price;
public:
    Order(char action, std::string ticker, int32_t quantity, double price);
    virtual ~Order() = default; // Virtual destructor for proper cleanup in derived classes

    virtual bool validateRules() const = 0; // Pure virtual function for validating order rules
    virtual void execute()  = 0; // Pure virtual function for printing order details


    // Getters for order attributes
    char getAction() const{return m_action;} // Getter for action
    std::string getTicker() const{return m_ticker;} // Getter for ticker
    int32_t getQuantity() const{return m_quantity;} // Getter for quantity
    double getPrice() const{return m_price;} // Getter for price

};

// Inheritance: Derived realization of a Limit Order (Must meet specific price floor constraints)
class LimitOrder : public Order {
public:
    using Order::Order; // Inherit constructors from the base class
    bool validateRules() const override; // Override for validating limit order rules
    void execute() override; // Override for executing limit order
};

// Inheritance: Derived realization of a Market Order (Executes instantly at best current matching fill)
class MarketOrder : public Order {
public:
    using Order::Order;
    bool validateRules() const override; // Override for validating market order rules
    void execute() override; // Override for executing market order
};


class OrderFactory {
public:
    static std::unique_ptr<Order> createOrder(const OrderPacket& packet); // Factory method to create orders based on packet data
};

}

#endif //LTS_ORDER_H
