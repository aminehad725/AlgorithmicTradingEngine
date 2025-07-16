#pragma once

#include <deque>
#include <cstddef>
#include "strategy_base.hpp"

class BollingerBandStrategy : public StrategyBase {
public:
    BollingerBandStrategy(std::size_t period, double numStdDev);
    OrderType generateSignal(const PriceData& data, double currentCash, int currentShares) override;

private:
    std::size_t period;
    double numStdDev;
    std::deque<double> prices;
};