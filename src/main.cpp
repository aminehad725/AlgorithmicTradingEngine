#include <iostream>
#include "../include/market_data.hpp"
#include "../include/strategy.hpp"
#include "../include/order.hpp"
#include "../include/portfolio.hpp"

int main() {
    std::string filename = "data/AAPL.csv";

    MarketDataLoader loader("data/AAPL.csv");
    auto data = loader.loadData(); 
    Strategy smaStrategy(10U, 20U); 
    Portfolio portfolio(100000, 1.00);

    for (const auto& dataPoint : data) { 
        auto signal = smaStrategy.generateSignal(dataPoint, portfolio.getCash(), portfolio.getShares());
        if (signal != OrderType::HOLD) {
            Order order(dataPoint.date, dataPoint.symbol, signal, 100, dataPoint.close);
            portfolio.executeOrder(order);
        }
        portfolio.updateMarketValue(dataPoint);
    }

    portfolio.printPerformanceReport();
    return 0;
}