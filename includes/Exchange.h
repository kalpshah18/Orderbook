#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>

#include "Orderbook.h"
#include "Definitions.h"

using SecurityId = std::string;

class Exchange
{
private:
    std::unordered_map<SecurityId, std::unique_ptr<Orderbook>> orderbooks_;
    mutable std::mutex exchangeMutex_;

public:
    Exchange() = default;
    Exchange(const Exchange&) = delete;
    void operator=(const Exchange&) = delete;
    Exchange(Exchange&&) = delete;
    void operator=(Exchange&&) = delete;
    ~Exchange() = default;

    void AddSecurity(const SecurityId& securityId);
    
    void RemoveSecurity(const SecurityId& securityId);
    
    Orderbook* GetOrderbook(const SecurityId& securityId);
    const Orderbook* GetOrderbook(const SecurityId& securityId) const;
    
    bool HasSecurity(const SecurityId& securityId) const;
    
    std::vector<SecurityId> GetSecurities() const;
    
    Trades AddOrder(const SecurityId& securityId, OrderPtr order);
    void CancelOrder(const SecurityId& securityId, OrderId orderId);
    Trades ModifyOrder(const SecurityId& securityId, OrderModify order);
    
    OrderBookLevelInfos GetOrderInfos(const SecurityId& securityId) const;
    std::size_t GetOrderbookSize(const SecurityId& securityId) const;
};
