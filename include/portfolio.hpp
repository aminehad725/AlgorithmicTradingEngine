#pragma once
#include <string>
#include <vector>
#include "order.hpp"
#include "market_data.hpp"

class Portfolio {
private:
    double cash;
    int shares;
    double marketValue;
    std::vector<Order> orderHistory;

public:
    Portfolio(double initialCash);
    void executeOrder(const Order& order);
    void updateMarketValue(const PriceData& data);
    void printSummary();

    double getCash() const { return cash; }
    int getShares() const { return shares; }
};