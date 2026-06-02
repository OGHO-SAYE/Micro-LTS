#include "lts/trading_engine.h"
#include "lts/risk_manager.h"
#include "lts/order.h"


#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>


namespace lts {

    class TradingEngine::Impl {
    private:
        RiskEngine m_riskEngine; // Risk engine to validate orders
        std::atomic<bool> m_running{false}; // Flag to control the engine's running state
        std::thread m_networkThread; // Thread for handling network connections
        int m_serverSocket{-1}; // Socket for accepting client connections

        // Low-latency DOD concept placeholder: pre-allocated continuous memory area
    // Instead of instantiating components inside loops, orders route directly here
    std::vector<std::unique_ptr<Order>> m_orderArena;

    public:
        Impl(double maxRiskValue, int32_t maxRiskQty) : m_riskEngine(maxRiskValue, maxRiskQty) {
            // Initialize the order arena with a fixed size for low-latency processing
            m_orderArena.reserve(1000); // Reserve space for 1000 orders
        }

        ~Impl() {
            stop(); // Ensure resources are cleaned up on destruction
        }

        void start(int port) {
            if (m_running) return; // Prevent multiple starts
            m_running = true;

            //setup local loopback TCP network stack parameters
            m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (m_serverSocket < 0) {
                std::cerr << "Failed to create socket" << std::endl;
                return;
            }

            int opt = 1;
            setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR, &opt, sizeof(opt));

            sockaddr_in serverAddr{};
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = INADDR_ANY;
            serverAddr.sin_port = htons(port);

            if(bind(m_serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
                std::cerr << "[ENGINE ERROR] Socket binding failed on port " << port << ".\n";                close(m_serverSocket);
                return;
            }
            if(listen(m_serverSocket, 3) < 0) {
                std::cerr << "[ENGINE ERROR] Socket listen rules failed.\n";                
                return;
            }

            std::cout << "[SYSTEM INITIALIZED] Engine listening for SBE packets on port " << port << "...\n";


            m_networkThread = std::thread(&Impl::networkListenLoop, this);
        }

        void stop() {
            if (!m_running) return; // Prevent multiple stops
            m_running = false;

            if(m_serverSocket >= 0) {
                close(m_serverSocket); // Close the server socket to unblock accept()
                m_serverSocket = -1;
            }
            if (m_networkThread.joinable()) {
                m_networkThread.join(); // Wait for the network thread to finish
            }
            std::cout << "[SYSTEM SHUTDOWN] Trading engine stopped successfully.\n";
        }

    private:
    void networkListenLoop() {
        while (m_running) {
            sockaddr_in clientAddr{};
            socklen_t addrLen = sizeof(clientAddr);
            
            int clientFd = accept(m_serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
            if (clientFd < 0) {
                if (!m_running) break; // Engine stopped safely
                continue;
            }

            std::cout << "[NETWORK CO-LOCATION] Connection accepted from market data provider.\n";

            OrderPacket buffer{};
            // Process sequential network packets over the wire directly into our struct memory space
            while (m_running && recv(clientFd, &buffer, sizeof(OrderPacket), MSG_WAITALL) > 0) {
                
                // Zero-Copy Direct Mapping Interface: Object parsing decoupling via Factory
                std::unique_ptr<Order> newOrder = OrderFactory::createOrder(buffer);

                if (newOrder) {
                    // Evaluate risk metrics via our static CRTP engine modules
                    if (m_riskEngine.validateOrder(*newOrder)) {
                        newOrder->execute();
                        
                        // Performance optimization: Push ownership cleanly into pre-allocated memory pools
                        m_orderArena.push_back(std::move(newOrder));
                    }
                }
            }
            close(clientFd);
            std::cout << "[NETWORK] Market data client disconnected.\n";
        }
    }
};

TradingEngine::TradingEngine(double maxRiskValue, int32_t maxRiskQty)
    : m_pImpl(std::make_unique<Impl>(maxRiskValue, maxRiskQty)) {}

TradingEngine::~TradingEngine() = default;

void TradingEngine::start(int port) { m_pImpl->start(port); }
void TradingEngine::stop() { m_pImpl->stop(); }

} // namespace lts