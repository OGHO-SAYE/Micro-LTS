#include "include/lts/risk_manager.h"

namespace lts {

// --- MaxOrderValueCheck Implementation ---
MaxOrderValueCheck::MaxOrderValueCheck(double maxValue) : m_maxValue(maxValue) {}

bool MaxOrderValueCheck::implementationCheck(const Order& order) {
    // For Market Orders with a 0.0 price token, we pass the risk check 
    // to let the matching engine sweep current depth.
    if (order.getPrice() == 0.0) {
        return true;
    }
    
    double orderValue = order.getQuantity() * order.getPrice();
    if (orderValue > m_maxValue) {
        std::cerr << "[RISK REJECTION] Order value $" << orderValue 
                  << " exceeds max allowed limit of $" << m_maxValue << "\n";
        return false;
    }
    return true;
}

// --- MaxOrderQuantityCheck Implementation ---
MaxOrderQuantityCheck::MaxOrderQuantityCheck(int32_t maxQuantity) : m_maxQuantity(maxQuantity) {}

bool MaxOrderQuantityCheck::implementationCheck(const Order& order) {
    if (order.getQuantity() > m_maxQuantity) {
        std::cerr << "[RISK REJECTION] Order quantity " << order.getQuantity() 
                  << " exceeds max allowed limit of " << m_maxQuantity << "\n";
        return false;
    }
    return true;
}

// --- RiskEngine Composite Implementation ---
RiskEngine::RiskEngine(double maxValue, int32_t maxQuantity)
    : m_valueCheck(maxValue), m_quantityCheck(maxQuantity) {}

bool RiskEngine::validateOrder(const Order& order) {
    // Execute both static strategies sequentially.
    // Notice that there are no runtime dynamic_casts or vtable pointer lookups here.
    return m_valueCheck.checkRisk(order) && m_quantityCheck.checkRisk(order);
}

} // namespace lts