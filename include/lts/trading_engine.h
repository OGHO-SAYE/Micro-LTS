#ifndef LTS_TRADING_ENGINE_H
#define LTS_TRADING_ENGINE_H

#include <memory>



namespace lts {

    class TradingEngine {
    private: 
        // Private members for order management, market data, etc. can be added here
        class Impl;
        std::unique_ptr<Impl> m_pImpl; // Pimpl idiom to hide implementation details
    
    public:
        TradingEngine(double maxRiskValue, int32_t maxRiskQty); // Constructor to initialize the trading engine with risk parameters
        ~TradingEngine(); // Destructor to clean up resources
    
        // Deleted copy semantics to ensure engine singletons can never be cloned
        TradingEngine(const TradingEngine&) = delete;
        TradingEngine& operator=(const TradingEngine&) = delete;
    

        // Starts the trading engine, potentially initializing network connections, threads, etc.
        void start(int port);

        // Stops the trading engine, cleaning up resources and shutting down connections
        void stop();
    };
} // namespace lts

#endif // LTS_TRADING_ENGINE_H