#pragma once

#include <deque>
#include <cstddef>
#include "strategy_base.hpp"

class SmaCrossoverStrategy : public StrategyBase {
public:
    SmaCrossoverStrategy(std::size_t shortW, std::size_t longW);
    OrderType generateSignal(const PriceData& data, double currentCash, int currentShares) override;

private:
    std::size_t shortWindow;
    std::size_t longWindow;
    std::deque<double> shortPrices;
    std::deque<double> longPrices;
};