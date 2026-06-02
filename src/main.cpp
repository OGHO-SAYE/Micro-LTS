#include "lts/trading_engine.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    std::cout << "====================================================\n";
    std::cout << "        LAUNCHING MICRO-LTS EXECUTION BACKEND       \n";
    std::cout << "====================================================\n";

    // Set engine safety rules: Max single order value $50,000 | Max order quantity 180 shares
    double maxOrderValue = 50000.0;
    int32_t maxOrderQty = 180;

    // Instantiate engine infrastructure
    lts::TradingEngine engine(maxOrderValue, maxOrderQty);

    // Boot up network listeners on local loopback interface port 8888
    engine.start(8888);

    std::cout << "[OPERATOR RUNNING] Processing engine active. Press Ctrl+C to terminate application.\n";

    // Simulate steady-state operational execution window
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}