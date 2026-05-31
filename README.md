# Micro-LTS: High-Performance Low-Latency Trading Infrastructure

Micro-LTS is a minimalist, ultra-low latency trading infrastructure prototype designed to demonstrate modern C++ system engineering paradigms, deterministic memory structures, and seamless cross-language scripting interop.

The system utilizes a split-architecture pattern: a lightweight Python injector packages and streams market data feeds using custom binary layouts, while a multi-threaded C++ engine handles high-performance data ingestion, pre-trade risk management, and order-book routing.

## 🚀 Key Architectural Paradigms & C++ Focus

This project is engineered to prioritize CPU cache efficiency and deterministic execution paths over conventional abstraction overhead:

- **Zero-Copy Binary Serialization:** Bypasses sluggish text parsers (JSON/XML) by matching Python's binary layout bit-for-bit using `#pragma pack` directives. Network byte buffers are pointer-cast directly to structure representations.
- **Modern Memory Ownership:** Utilizes strict single-ownership strategies via `std::unique_ptr` and zero-overhead move semantics (`std::move`) to transition order lifecycles across concurrent boundaries.
- **The Pimpl Idiom (Pointer to Implementation):** Decouples public API interfaces from internal compilation dependencies, hiding private containers and thread hooks to achieve blistering compilation turnarounds.
- **Object-Oriented Design & Factory Patterns:** Implements robust abstract base boundaries for polymorphic runtime behavior while leveraging an `OrderFactory` to isolate allocation mechanics.
- **Data-Oriented Design (DOD) Prep:** Structured specifically to pivot away from pointer-chasing, node-based collections (e.g., `std::list`), favoring contiguous memory vectors to maximize L1/L2 cache locality.

---

## 📂 System Directory Structure

```text
micro-lts/
├── cmake/                      # CMake configuration helpers
├── CMakeLists.txt              # Root build configuration
├── README.md                   # Project documentation
│
├── scripts/                    # Scripting, Ingestion & Tooling
│   ├── data/
│   │   └── mock_market_data.csv# Input CSV tick sequences
│   └── market_injector.py      # Python binary socket blaster (SBE Mock)
│
├── include/                    # Public Interface Headers
│   └── lts/
│       ├── trading_engine.h    # Public Core API (uses Pimpl pattern)
│       ├── order.h             # Order abstractions & binary network structures
│       └── risk_manager.h      # Compile-time risk verification interface
│
├── src/                        # Implementation Layer
│   ├── trading_engine.cpp      # Decoupled Engine core logic & ring buffer loops
│   ├── order.cpp               # Order rules & Factory implementation
│   └── main.cpp                # Main app entry point & network gateway setup
│
└── tests/                      # Automated Verification (GoogleTest)
    ├── CMakeLists.txt          # Test build suite hooks
    ├── test_order_book.cpp     # Deterministic DOD validation suites
    └── test_risk_manager.cpp   # Validation checks for risk routines
```
