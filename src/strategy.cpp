#include "../include/strategy.hpp"
#include <iostream>

Strategy::Strategy(std::size_t shortW, std::size_t longW) 
    : shortWindow(shortW), longWindow(longW) {}

OrderType Strategy::generateSignal(const PriceData& data, double currentCash, int currentShares) {
    shortPrices.push_back(data.close);
    longPrices.push_back(data.close);

    if (shortPrices.size() > shortWindow) shortPrices.pop_front();
    if (longPrices.size() > longWindow) longPrices.pop_front();

    if (shortPrices.size() < shortWindow || longPrices.size() < longWindow)
        return OrderType::HOLD;

    double shortAvg = 0, longAvg = 0;
    for (double p : shortPrices) shortAvg += p;
    for (double p : longPrices) longAvg += p;
    shortAvg /= shortPrices.size();
    longAvg /= longPrices.size();

    const int tradeQuantity = 100; 

    if (shortAvg > longAvg) {
        if (currentCash >= tradeQuantity * data.close) { 
            return OrderType::BUY;
        }
    } else if (shortAvg < longAvg) {
        if (currentShares >= tradeQuantity) { 
            return OrderType::SELL;
        }
    }
    return OrderType::HOLD;
}