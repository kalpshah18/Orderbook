// OrderModify.h
// File to define the OrderModify class, which represents a modification to an existing order in the order book system.

#pragma once
#include "Definitions.h"
#include "Order.h"

#include <memory>

class OrderModify
{
public:
	OrderModify(OrderId orderId, Price price, Quantity quantity, Side side)
		: orderId_{ orderId },
		  price_{ price },
		  quantity_{ quantity },
          side_{ side } {}
		
		  OrderId GetOrderId() const {
			return orderId_;
		  }
		  Price GetPrice() const {
			return price_;
		  }
		  Quantity GetQuantity() const {
			return quantity_;
		  }		  Side GetSide() const {
			return side_;
		  }
	
	OrderPtr ToOrderPointer(OrderType type) const{
		return std::make_shared<Order>(type, GetOrderId(), GetSide(), GetPrice(), GetQuantity());
	}

private:
	OrderId orderId_;
	Price price_;
	Quantity quantity_;
	Side side_;
};