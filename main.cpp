// main.cpp
// File to Create and Use Exchange with Orderbooks
// Command: g++ -std=c++20 main.cpp Orderbook.cpp Exchange.cpp -o main.exe

#include "includes/Exchange.h"
#include "includes/Order.h"
#include <iostream>

int main(){
    Exchange exchange;    // Create an Exchange
    
    // Add securities to the exchange
    exchange.AddSecurity("AAPL");
    exchange.AddSecurity("GOOGL");
    exchange.AddSecurity("MSFT");
    
    std::cout << "Exchange created with securities: ";
    auto securities = exchange.GetSecurities();
    for (const auto& security : securities) {
        std::cout << security << " ";
    }
    std::cout << std::endl;
    Orderbook* aaplOrderbook = exchange.GetOrderbook("AAPL");
    if (aaplOrderbook) {
        std::cout << "AAPL orderbook size: " << aaplOrderbook->Size() << std::endl;
    }
    std::cout << "Has AAPL: " << (exchange.HasSecurity("AAPL") ? "Yes" : "No") << std::endl;
    std::cout << "Has TSLA: " << (exchange.HasSecurity("TSLA") ? "Yes" : "No") << std::endl;
    
    return 0;
}