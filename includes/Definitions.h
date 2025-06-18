// Defintions.h
// File to define types for clarity
// and store common Structs and Enums

#pragma once
#include <cstdint>

using Price = std::int32_t;
using Quantity = std::uint32_t;
using OrderId = std::uint64_t;

enum class OrderType {
	GoodTillCancel,
	FillAndKill
};

enum class Side
{
	Buy,
	Sell
};