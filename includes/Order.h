// Order.h
// File to define the Order class, which represents an order in the order book system.

#pragma once

#include <memory>
#include <vector>
#include <stdexcept>
#include <format>
#include "Definitions.h"

class Order {
    public:
        Order(OrderType orderType, OrderId orderId, Side side, Price price, Quantity quantity)
        : orderType_(orderType), orderId_(orderId), side_(side), price_(price), initialQuantity_(quantity), remainingQuantity_(quantity) {}

        Order(OrderId orderId, Side side, Quantity quantity)
        : Order(OrderType::Market, orderId, side, Constants::InvalidPrice, quantity) {}

        OrderType GetOrderType() const {
            return orderType_;
        }
        OrderId GetOrderId() const {
            return orderId_;
        }
        Side GetSide() const {
            return side_;
        }
        Price GetPrice() const {
            return price_;
        }
        Quantity GetInitialQuantity() const {
            return initialQuantity_;
        }
        Quantity GetRemainingQuantity() const {
            return remainingQuantity_;
        }
        Quantity GetFilledQuantity() const {
            return initialQuantity_ - remainingQuantity_;
        }
        bool IsFilled() const {
            return remainingQuantity_ == 0;
        }
        void Fill(Quantity quantity) {
            if (quantity > remainingQuantity_) {
                throw std::logic_error(std::format("Cannot fill {} for order {}. Remaining quantity is only {}.", quantity, orderId_, remainingQuantity_));
            }
            remainingQuantity_ -= quantity;
        }
        void ToGoodTillCancel(Price price)
        {
            if (GetOrderType() == OrderType::Market) throw std::logic_error(std::format("Order ({}) cannot have its price adjusted, only market orders can.", GetOrderId()));

            price_ = price;
            orderType_ = OrderType::GoodTillCancel;
        }
      private:
        OrderType orderType_;
        OrderId orderId_;
        Side side_;
        Price price_;
        Quantity initialQuantity_;
        Quantity remainingQuantity_;
    };

using OrderPtr = std::shared_ptr<Order>;
using OrderPtrList = std::vector<OrderPtr>;