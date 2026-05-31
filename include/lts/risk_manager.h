#ifndef LTS_RISK_MANAGER_H
#define LTS_RISK_MANAGER_H

#include "lts/order.h"
#include <iostream>

namespace lts {

    // CRTP Base Template: Achieves compile-time polymorphism, eliminating vtable lookup overhead
    template<typename Derived>
    class RiskCheck{
        public:
            bool checkRisk(const Order& order) {
                // Delegates to the derived class's implementation
                return static_cast<Derived*>(this)->implementationCheck(order);
            }

    };

    // Example Risk Check: Validates that the total order value does not exceed a specified maximum
    class MaxOrderValueCheck : public RiskCheck<MaxOrderValueCheck> {
        private: 
            double m_maxValue;

        public:
           explicit MaxOrderValueCheck(double maxValue);
           bool implementationCheck(const Order& order);
    };

    // Example Risk Check: Validates that the order quantity does not exceed a specified maximum
    class MaxOrderQuantityCheck : public RiskCheck<MaxOrderQuantityCheck> {
        private:
            int32_t m_maxQuantity;

        public:
            explicit MaxOrderQuantityCheck(int32_t maxQuantity);
            bool implementationCheck(const Order& order);
        
    };

    class RiskEngine {
        private:
            MaxOrderValueCheck m_valueCheck; // Risk check for maximum order value
            MaxOrderQuantityCheck m_quantityCheck; // Risk check for maximum order quantity
            
        public: 
            RiskEngine(double maxValue, int32_t maxQuantity);
            bool validateOrder(const Order& order); // Validates an order against all risk checks
    };

}

#endif //LTS_RISK_MANAGER_H
