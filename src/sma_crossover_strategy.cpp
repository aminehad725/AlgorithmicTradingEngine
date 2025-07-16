#include "../include/sma_crossover_strategy.hpp"
#include <numeric>

SmaCrossoverStrategy::SmaCrossoverStrategy(std::size_t shortW, std::size_t longW) 
    : shortWindow(shortW), longWindow(longW) {}

OrderType SmaCrossoverStrategy::generateSignal(const PriceData& data, double currentCash, int currentShares) {
    shortPrices.push_back(data.close);
    longPrices.push_back(data.close);

    if (shortPrices.size() > shortWindow) {
        shortPrices.pop_front();
    }
    if (longPrices.size() > longWindow) {
        longPrices.pop_front();
    }

    if (longPrices.size() < longWindow) {
        return OrderType::HOLD;
    }

    double shortAvg = std::accumulate(shortPrices.begin(), shortPrices.end(), 0.0) / shortPrices.size();
    double longAvg = std::accumulate(longPrices.begin(), longPrices.end(), 0.0) / longPrices.size();

    if (shortAvg > longAvg) {
        if (currentShares == 0 && currentCash >= 100 * data.close) {
            return OrderType::BUY;
        }
    } else if (shortAvg < longAvg) {
        if (currentShares > 0) {
            return OrderType::SELL;
        }
    }
    
    return OrderType::HOLD;
}