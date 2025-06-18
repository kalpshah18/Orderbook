// OrderBookLevelInfos.h
// File to define the OrderBookLevelInfos class, which holds bid and ask level information in the order book system.

#pragma once
#include "Definitions.h"

#include <vector>

struct LevelInfo
{
	Price price_;
	Quantity quantity_;
};

using LevelInfos = std::vector<LevelInfo>;

class OrderBookLevelInfos
{
	public:
		OrderBookLevelInfos(const LevelInfos& bids, const LevelInfos& asks)
		: bids_(bids), asks_(asks) {}

	const LevelInfos& GetBids() const {
		return bids_;
	}

	const LevelInfos& GetAsks() const {
		return asks_;
	}

	private:
		LevelInfos bids_;
		LevelInfos asks_;
};