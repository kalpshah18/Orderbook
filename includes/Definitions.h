// Defintions.h
// File to define types for clarity
// and store common Structs and Enums

#pragma once
#include <cstdint>
#include <limits>
#include <vector>

using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;
using OrderIds = std::vector<OrderId>;

struct Constants{
	static const Price InvalidPrice = std::numeric_limits<Price>::quiet_NaN();
};

enum class OrderType {
	GoodTillCancel,
	FillAndKill,
	Market,
	GoodForDay,
	FillOrKill
};

enum class Side
{
	Buy,
	Sell
};