#include <iostream>
#include "../include/market_data.hpp"
#include "../include/strategy_base.hpp"
#include "../include/sma_crossover_strategy.hpp"
#include "../include/bollinger_band_strategy.hpp"
#include "../include/order.hpp"
#include "../include/portfolio.hpp"

void runBacktest(StrategyBase& strategy, const std::vector<PriceData>& data) {
    Portfolio portfolio(100000, 1.00);
    for (const auto& dataPoint : data) {
        auto signal = strategy.generateSignal(dataPoint, portfolio.getCash(), portfolio.getShares());
        if (signal != OrderType::HOLD) {
            Order order(dataPoint.date, dataPoint.symbol, signal, 100, dataPoint.close);
            portfolio.executeOrder(order);
        }
        portfolio.updateMarketValue(dataPoint);
    }
    portfolio.printPerformanceReport();
}

int main() {
    MarketDataLoader loader("data/AAPL.csv");
    auto data = loader.loadData();

    SmaCrossoverStrategy smaStrategy(50, 200);
    BollingerBandStrategy bbandStrategy(20, 2.0);

    std::cout << "\n--- Testing SMA Crossover Strategy ---\n";
    runBacktest(smaStrategy, data);

    std::cout << "\n--- Testing Bollinger Band Strategy ---\n";
    runBacktest(bbandStrategy, data);

    return 0;
}