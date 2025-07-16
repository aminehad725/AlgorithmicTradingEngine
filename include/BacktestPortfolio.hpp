#pragma once

#include <vector>
#include <string>
#include <map>
#include "PortfolioBase.hpp"
#include "market_data.hpp"

class BacktestPortfolio : public PortfolioBase {
public:
    BacktestPortfolio(double initialCash, double costPerTrade);
    void executeOrder(const Order& order) override;
    double getCash() const override;
    int getShares(const std::string& symbol) const override;
    void updateMarketValue(const std::map<std::string, PriceData>& latestPrices);
    void printPerformanceReport();

private:
    double cash;
    std::map<std::string, int> positions;
    double initialCapital;
    std::vector<Order> orderHistory;
    std::vector<double> historicalValues;
    double transactionCost;
};