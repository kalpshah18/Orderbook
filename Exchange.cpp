#include "includes/Exchange.h"
#include <stdexcept>

void Exchange::AddSecurity(const SecurityId& securityId)
{
    std::lock_guard<std::mutex> lock(exchangeMutex_);
    
    if (orderbooks_.find(securityId) != orderbooks_.end())
    {
        throw std::invalid_argument("Security " + securityId + " already exists");
    }
    
    orderbooks_[securityId] = std::make_unique<Orderbook>();
}

void Exchange::RemoveSecurity(const SecurityId& securityId)
{
    std::lock_guard<std::mutex> lock(exchangeMutex_);
    
    auto it = orderbooks_.find(securityId);
    if (it == orderbooks_.end())
    {
        throw std::invalid_argument("Security " + securityId + " not found");
    }
    
    orderbooks_.erase(it);
}

Orderbook* Exchange::GetOrderbook(const SecurityId& securityId)
{
    std::lock_guard<std::mutex> lock(exchangeMutex_);
    
    auto it = orderbooks_.find(securityId);
    if (it == orderbooks_.end())
    {
        return nullptr;
    }
    
    return it->second.get();
}

const Orderbook* Exchange::GetOrderbook(const SecurityId& securityId) const
{
    std::lock_guard<std::mutex> lock(exchangeMutex_);
    
    auto it = orderbooks_.find(securityId);
    if (it == orderbooks_.end())
    {
        return nullptr;
    }
    
    return it->second.get();
}

bool Exchange::HasSecurity(const SecurityId& securityId) const
{
    std::lock_guard<std::mutex> lock(exchangeMutex_);
    return orderbooks_.find(securityId) != orderbooks_.end();
}

std::vector<SecurityId> Exchange::GetSecurities() const
{
    std::lock_guard<std::mutex> lock(exchangeMutex_);
    
    std::vector<SecurityId> securities;
    securities.reserve(orderbooks_.size());
    
    for (const auto& pair : orderbooks_)
    {
        securities.push_back(pair.first);
    }
    
    return securities;
}

Trades Exchange::AddOrder(const SecurityId& securityId, OrderPtr order)
{
    Orderbook* orderbook = GetOrderbook(securityId);
    if (!orderbook)
    {
        throw std::invalid_argument("Security " + securityId + " not found");
    }
    
    return orderbook->AddOrder(order);
}

void Exchange::CancelOrder(const SecurityId& securityId, OrderId orderId)
{
    Orderbook* orderbook = GetOrderbook(securityId);
    if (!orderbook)
    {
        throw std::invalid_argument("Security " + securityId + " not found");
    }
    
    orderbook->CancelOrder(orderId);
}

Trades Exchange::ModifyOrder(const SecurityId& securityId, OrderModify order)
{
    Orderbook* orderbook = GetOrderbook(securityId);
    if (!orderbook)
    {
        throw std::invalid_argument("Security " + securityId + " not found");
    }
    
    return orderbook->ModifyOrder(order);
}

OrderBookLevelInfos Exchange::GetOrderInfos(const SecurityId& securityId) const
{
    const Orderbook* orderbook = GetOrderbook(securityId);
    if (!orderbook)
    {
        throw std::invalid_argument("Security " + securityId + " not found");
    }
    
    return orderbook->GetOrderInfos();
}

std::size_t Exchange::GetOrderbookSize(const SecurityId& securityId) const
{
    const Orderbook* orderbook = GetOrderbook(securityId);
    if (!orderbook)
    {
        throw std::invalid_argument("Security " + securityId + " not found");
    }
    
    return orderbook->Size();
}
