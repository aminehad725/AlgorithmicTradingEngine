#pragma once
#include <cstddef>
#include <deque>
#include "market_data.hpp"
#include "order.hpp"

class Strategy {
private:
    std::size_t shortWindow, longWindow;
    std::deque<double> shortPrices, longPrices;

public:
    Strategy(std::size_t shortW, std::size_t longW);
    OrderType generateSignal(const PriceData& data, double currentCash, int currentShares);
};