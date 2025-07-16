#pragma once

#include "market_data.hpp"
#include "order.hpp"

class StrategyBase {
public:
    virtual ~StrategyBase() = default;
    virtual OrderType generateSignal(const PriceData& data, double currentCash, int currentShares) = 0;
};