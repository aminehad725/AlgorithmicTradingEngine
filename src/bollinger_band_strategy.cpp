#include "../include/bollinger_band_strategy.hpp"
#include <numeric>
#include <cmath>

BollingerBandStrategy::BollingerBandStrategy(std::size_t p, double n) 
    : period(p), numStdDev(n) {}

OrderType BollingerBandStrategy::generateSignal(const PriceData& data, double currentCash, int currentShares) {
    prices.push_back(data.close);
    if (prices.size() > period) {
        prices.pop_front();
    }

    if (prices.size() < period) {
        return OrderType::HOLD;
    }

    double sum = std::accumulate(prices.begin(), prices.end(), 0.0);
    double middleBand = sum / prices.size();

    double sq_sum = 0.0;
    for(const auto& p : prices) {
        sq_sum += (p - middleBand) * (p - middleBand);
    }
    double stdDev = std::sqrt(sq_sum / prices.size());

    double upperBand = middleBand + (numStdDev * stdDev);
    double lowerBand = middleBand - (numStdDev * stdDev);

    if (data.close < lowerBand) {
        if (currentShares == 0 && currentCash >= 100 * data.close) {
            return OrderType::BUY;
        }
    } else if (data.close > upperBand) {
        if (currentShares > 0) {
            return OrderType::SELL;
        }
    }

    return OrderType::HOLD;
}