#pragma once
#include <vector>
#include "order.hpp"
#include "market_data.hpp"

class Portfolio {
private:
    double cash;
    int shares;
    double initialCapital;
    std::vector<Order> orderHistory;
    std::vector<double> historicalValues;
    double transactionCost;

public:
    Portfolio(double initialCash, double costPerTrade);
    void executeOrder(const Order& order);
    void updateMarketValue(const PriceData& data);
    void printPerformanceReport();

    double getCash() const { return cash; }
    int getShares() const { return shares; }
};