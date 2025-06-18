// Orderbook.h
// File to define the Orderbook class, which manages orders and trades in the order book system.

#pragma once

#include "Order.h"
#include "OrderModify.h"
#include "OrderbookLevelInfos.h"
#include "Trade.h"
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <format>
#include <numeric>

class Orderbook {
    private:
        struct OrderEntry {
            OrderPtr order_{nullptr};
            OrderPtrList::iterator location_;
        };

        std::map<Price, OrderPtrList, std::greater<Price>> bids_;
        std::map<Price, OrderPtrList, std::less<Price>> asks_;
        std::unordered_map<OrderId, OrderEntry> orders_;

        bool can_match(Side side, Price price) const {
            if(side == Side::Buy){
                if(asks_.empty() || price < asks_.begin()->first) {
                    return false;
                }
                return true;
            } else {
                if(bids_.empty() || price > bids_.begin()->first) {
                    return false;
                }
                return true;
            }
        }

        Trades MatchOrders(){
            Trades trades;
            trades.reserve(orders_.size());

            while (true){
                if(bids_.empty() || asks_.empty()) break;

                auto& [bestBid, bids] = *bids_.begin();
                auto& [bestAsk, asks] = *asks_.begin();

                if(bestBid < bestAsk) break;

                while(bids.size() && asks.size()){
                    auto& bid = bids.front();
                    auto& ask = asks.front();

                    Quantity quantity = std::min(bid->GetRemainingQuantity(), ask->GetRemainingQuantity());

                    bid->Fill(quantity);
                    ask->Fill(quantity);

                    if(bid->IsFilled()) {
                        bids.erase(bids.begin());
                        orders_.erase(ask->GetOrderId());
                    }

                    if(ask->IsFilled()) {
                        asks.erase(asks.begin());
                        orders_.erase(bid->GetOrderId());
                    }

                    if (bids.empty()) bids_.erase(bestBid);
                    if (asks.empty()) asks_.erase(bestAsk);

                    trades.push_back(Trade{
                        TradeInfo{bid->GetOrderId(), bid->GetPrice(), quantity},
                        TradeInfo{ask->GetOrderId(), ask->GetPrice(), quantity}
                    });

                    if (!bids_.empty())
                    {
                        auto& [_, bids] = *bids_.begin();
                        auto& order = bids.front();
                        if (order->GetOrderType() == OrderType::FillAndKill) CancelOrder(order->GetOrderId());
                    }
                    if(!asks_.empty())
                    {
                        auto& [_, asks] = *asks_.begin();
                        auto& order = asks.front();
                        if (order->GetOrderType() == OrderType::FillAndKill) CancelOrder(order->GetOrderId());
                    }

                    return trades;
                }
            }
        }

        public:
            Trades AddOrder(OrderPtr order){
                if (orders_.contains(order->GetOrderId())) {
                    return {};
                }

                if (order->GetOrderType() == OrderType::FillAndKill && !can_match(order->GetSide(), order->GetPrice())) {
                    return {};
                }

                OrderPtrList::iterator iterator;

                if (order->GetSide() == Side::Buy) {
                    auto& bids = bids_[order->GetPrice()];
                    iterator = bids.insert(bids.end(), order);
                    orders_[order->GetOrderId()] = {order, iterator};
                } else {
                    auto& asks = asks_[order->GetPrice()];
                    iterator = asks.insert(asks.end(), order);
                    orders_[order->GetOrderId()] = {order, iterator};
                }

                orders_.insert({order->GetOrderId(), OrderEntry{order, iterator}});

                return MatchOrders();
            }

            void CancelOrder(OrderId orderId) {
                if(!orders_.contains(orderId)) return;

                const auto& [order, iterator] = orders_.at(orderId);

                orders_.erase(orderId);

                if (order->GetSide() == Side::Sell){
                    auto price = order->GetPrice();
                    auto& orders = asks_.at(price);
                    orders.erase(iterator);
                    if (orders.empty()) {
                        asks_.erase(price);
                    }
                } else {
                    auto price = order->GetPrice();
                    auto& orders = bids_.at(price);
                    orders.erase(iterator);
                    if (orders.empty()) {
                        bids_.erase(price);
                    }
                }
            }

            Trades MatchOrder(OrderModify order){
                if(!orders_.contains(order.GetOrderId())) return {};

                const auto& [existingOrder, _] = orders_.at(order.GetOrderId());
                CancelOrder(order.GetOrderId());
                return AddOrder(order.ToOrderPointer(existingOrder->GetOrderType()));
            }

            std::size_t Size() const {
                return orders_.size();
            }

            OrderBookLevelInfos GetOrderInfos() const
            {
                LevelInfos bidInfos, askInfos;
                bidInfos.reserve(orders_.size());
                askInfos.reserve(orders_.size());

                auto CreateLevelInfos = [](Price price, const OrderPtrList& orders){
                    return LevelInfo{price, std::accumulate(orders.begin(), orders.end(), (Quantity) 0, 
                        [](Quantity runningSum, const OrderPtr& order)
                    { return runningSum + order->GetRemainingQuantity(); })};
                };

                for (const auto& [price, orders]: bids_) bidInfos.push_back(CreateLevelInfos(price, orders));
                for (const auto& [price, orders]: asks_) askInfos.push_back(CreateLevelInfos(price, orders));
                return OrderBookLevelInfos{ bidInfos, askInfos};
            }
};