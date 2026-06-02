#include "lts/order.h"
#include <cstring>

namespace lts {

    //Base class constructor implementation
    Order::Order(char action, std::string ticker, int32_t quantity, double price)
        : m_action(action), m_ticker(std::move(ticker)), m_quantity(quantity), m_price(price) {}

    // -- LimitOrder Implementation  validation and logic --
    bool LimitOrder::validateRules() const {
        // Limit orders must have a positive price and quantity, and action must be 'B'
        return m_quantity > 0 && m_price > 0.0;
    }

    void LimitOrder::execute() {
        std::cout << "Executing Limit Order: " << m_action << " " << m_quantity
                  << " shares of " << m_ticker << " @ $" << m_price << std::endl;
    }

    // -- MarketOrder Implementation  validation and logic --
    bool MarketOrder::validateRules() const {
        // Market orders must have a positive quantity, and action must be 'B' or '
        return m_quantity > 0;
    }

    void MarketOrder::execute() {
        std::cout << "Executing Market Order: " << m_action << " " << m_quantity
                  << " shares of " << m_ticker << " at market price" << std::endl;
    }



    // Factory method to create orders based on packet data
    std::unique_ptr<Order> OrderFactory::createOrder(const OrderPacket& packet) {
        // Convert fixed-size ticker to std::string, trimming any null characters
        std::string cleanTicker(packet.ticker, strnlen(packet.ticker, sizeof(packet.ticker)));


        // Simple structural routing logic based on mock execution types
        // Let's assume prices ending in whole integers represent market fills, others represent strict limits
        bool isMarketOrder = (packet.price == 0.0); // Market orders have price set to 0.0  

        if(isMarketOrder) {
            return std::make_unique<MarketOrder>(packet.action, std::move(cleanTicker), packet.quantity, packet.price);
        } else {
            return std::make_unique<LimitOrder>(packet.action, std::move(cleanTicker), packet.quantity, packet.price);
        }    
    }
    };
